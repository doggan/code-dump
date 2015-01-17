var encryption = require('./encryption');

var ID_MPQ = 0x1A51504D;  // MPQ archive header ID ('MPQ\x1A')
var MPQ_HEADER_SIZE_V1 = 0x20;

/**
* Returns an object with the following fields:
*  magic: Magic # id describing the file.
*  headerSize: The size in bytes of the header.
*  archiveSize: The size in bytes of the archive file.
*  formatVersion: The version of this archive file.
*  sectorSizeShift: Power of two exponent specifying the number of 512-byte disk
*                   sectors in each logical sector in the archive. The size of
*                   each logical sector in the archive is 512 * 2^wBlockSize.
*  hashTableOffset: Offset to the beginning of the hash table, relative to the
*                   beginning of the archive.
*  blockTableOffset: Offset to the beginning of the block table, relative to the
*                    beginning of the archive.
*  hashTableEntryCount: Number of entries in the hash table. Must be a power of two,
*                       and must be less than 2^16 for the original MPQ format,
*                       or less than 2^20 for the Burning Crusade format.
*  blockTableEntryCount: Number of entries in the block table.
*/
function parseHeader(mpqHandle) {
    var offset = 0;
    var res = {};

    console.log('parse header');
    var buffer = new Buffer(new Uint8Array(mpqHandle.rawBuffer, offset, MPQ_HEADER_SIZE_V1));
    console.log('parse done');

    res.magic = buffer.readUInt32LE(offset);
    if (res.magic !== ID_MPQ) {
        console.error('Invalid MPQ.');
        return false;
    }

    res.headerSize = buffer.readUInt32LE(offset += 4);
    res.archiveSize = buffer.readUInt32LE(offset += 4);
    res.formatVersion = buffer.readUInt16LE(offset += 4);

    if (res.formatVersion !== 0) {
        console.error('Unhandled MPQ version: [' + formatVersion + ']');
        return false;
    }

    if (res.headerSize !== MPQ_HEADER_SIZE_V1) {
        console.error('Invalid header size [' + headerSize + '] for MPQ version 1.');
        return false;
    }

    res.sectorSizeShift = buffer.readUInt16LE(offset += 2);
    res.hashTableOffset = buffer.readUInt32LE(offset += 2);
    res.blockTableOffset = buffer.readUInt32LE(offset += 4);
    res.hashTableEntryCount = buffer.readUInt32LE(offset += 4);
    res.blockTableEntryCount = buffer.readUInt32LE(offset += 4);

    return res;
}

/**
* Returns an array of hash tables within the archive file.
*
* A single entry in the array has the following structure:
*  hashA: The hash of the full file name (part A).
*  hashB: The hash of the full file name (part B).
*  locale: The language of the file. This is a Windows LANGID data type,
*          and uses the same values. 0 indicates the default language (American
*          English), or that the file is language-neutral.
*  platform: The platform the file is used for. 0 indicates the default platform.
*  blockTableIndex: If the hash table entry is valid, this is the index into the
*                   block table of the file.
*                   Otherwise, one of the following two values:
*                    - FFFFFFFF: Hash table entry is empty, and has always been empty.
*                      Terminates searches for a given file.
*                    - FFFFFFFE: Hash table entry is empty, but was valid at some point (a deleted file).
*                      Does not terminate searches for a given file.
*/
function parseHashTable(mpqHandle) {
    var res = [];

    var header = mpqHandle.header;
    var entryCount = header.hashTableEntryCount;
    var length = entryCount * 16;    // Single entry is 16 bytes.

    var buffer = new Buffer(new Uint8Array(mpqHandle.rawBuffer, header.hashTableOffset, length));

    // Decryption.
    var hashId = '(hash table)';
    var key = encryption.hash(hashId, 'TABLE');
    buffer = encryption.decrypt(buffer, key);

    var offset = 0;
    for (var i = 0; i < entryCount; i++) {
        res.push({
            hashA: buffer.readUInt32LE(offset),
            hashB: buffer.readUInt32LE(offset += 4),
            locale: buffer.readUInt16LE(offset += 4),
            platform: buffer.readUInt16LE(offset += 2),
            blockTableIndex: buffer.readUInt32LE(offset += 2)
        });

        offset += 4;
    }

    return res;
}

// offset: Offset of the beginning of the file data, relative to the beginning of the archive.
// compressedSize: Compressed file size.
// size: Size of uncompressed file.
// flags: Flags for the file.
function parseBlockTable(mpqHandle) {
    var res = [];

    var header = mpqHandle.header;
    var entryCount = header.blockTableEntryCount;
    var length = entryCount * 16;    // Single entry is 16 bytes.

    var buffer = new Buffer(new Uint8Array(mpqHandle.rawBuffer, header.blockTableOffset, length));

    // Decryption.
    var hashId = '(block table)';
    var key = encryption.hash(hashId, 'TABLE');
    buffer = encryption.decrypt(buffer, key);

    var offset = 0;
    for (var i = 0; i < entryCount; i++) {
        res.push({
            offset: buffer.readUInt32LE(offset),
            compressedSize: buffer.readUInt32LE(offset += 4),
            size: buffer.readUInt32LE(offset += 4),
            flags: buffer.readUInt32LE(offset += 4)
        });

        offset += 4;
    }

    return res;
}

function MPQHandle(rawBuffer) {
    this.rawBuffer = rawBuffer;

    this.header = parseHeader(this);
    console.log('Header: ' + JSON.stringify(this.header, null, '\t'));

    this.hashTable = parseHashTable(this);
    // console.log('HashTable: ' + JSON.stringify(this.hashTable, null, '\t'));
    console.log('HashTable[0]: ' + JSON.stringify(this.hashTable[0], null, '\t'));

    this.blockTable = parseBlockTable(this);
    console.log('BlockTable[0]: ' + JSON.stringify(this.blockTable[0], null, '\t'));
}

function getHashEntry(fileName, hashTable) {
    var hash_a = encryption.hash(fileName, 'HASH_A');
    var hash_b = encryption.hash(fileName, 'HASH_B');

    for (var i in hashTable) {
        if (hash_a === hashTable[i].hashA && hash_b === hashTable[i].hashB) {
            return hashTable[i];
        }
    }

    return null;
}

var MPQ_FILE_IMPLODE = 0x00000100;
var MPQ_FILE_COMPRESS = 0x00000200;
var MPQ_FILE_ENCRYPTED = 0x00010000;
var MPQ_FILE_FIX_KEY = 0x00020000;
var MPQ_FILE_SINGLE_UNIT = 0x01000000;
var MPQ_FILE_DELETE_MARKER = 0x02000000;
var MPQ_FILE_SECTOR_CRC = 0x04000000;
var MPQ_FILE_EXISTS = 0x80000000;

function read(fileName) {
    var result = {
        offset: 0,
    };

    var hashEntry = getHashEntry(fileName, this.hashTable);
    if (hashEntry === null) {
        console.log('HashEntry not found: ' + fileName);
        return result;
    }

    var blockEntry = this.blockTable[hashEntry.blockTableIndex];
    if (!(blockEntry.flags & MPQ_FILE_EXISTS)) {
        console.log('File does not exist: ' + fileName);
        return result;
    }

    if (blockEntry.compressedSize === 0) {
        console.log('Empty file.');
        return result;
    }

    console.log('MPQ_FILE_IMPLODE: ' + !!(blockEntry.flags & MPQ_FILE_IMPLODE));
    console.log('MPQ_FILE_COMPRESS: ' + !!(blockEntry.flags & MPQ_FILE_COMPRESS));
    console.log('MPQ_FILE_ENCRYPTED: ' + !!(blockEntry.flags & MPQ_FILE_ENCRYPTED));
    console.log('MPQ_FILE_FIX_KEY: ' + !!(blockEntry.flags & MPQ_FILE_FIX_KEY));
    console.log('MPQ_FILE_SINGLE_UNIT: ' + !!(blockEntry.flags & MPQ_FILE_SINGLE_UNIT));
    console.log('MPQ_FILE_DELETE_MARKER: ' + !!(blockEntry.flags & MPQ_FILE_DELETE_MARKER));
    console.log('MPQ_FILE_SECTOR_CRC: ' + !!(blockEntry.flags & MPQ_FILE_SECTOR_CRC));
    console.log('MPQ_FILE_EXISTS: ' + !!(blockEntry.flags & MPQ_FILE_EXISTS));

    // if (blockEntry.flags & MPQ_FILE_ENCRYPTED) {
    //     console.warn('Encryption not supported.');
    //     return result;
    // }

    result.offset = blockEntry.offset;
    result.length = blockEntry.size;

    return result;
}

function load(rawBuffer) {
    var mpqHandle = new MPQHandle(rawBuffer);
}

module.exports = {
    load: load,
    loadFromBuffer : load
};

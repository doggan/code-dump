var HASH_TYPES = {
    'TABLE_OFFSET': 0,
    'HASH_A': 1,
    'HASH_B': 2,
    'TABLE': 3
};

function buildEncryptionTable() {
    var seed = 0x00100001;
    var table = {};

    for (var i = 0; i < 256; i++) {
        index = i;
        for (var j = 0; j < 5; j++) {
            seed = (seed * 125 + 3) % 0x2AAAAB;
            var temp1 = ((seed & 0xFFFF) << 0x10);

            seed = (seed * 125 + 3) % 0x2AAAAB;
            var temp2 = (seed & 0xFFFF);

            // Use the >>> operator to prevent signed bit operations.
            table[index] = (temp1 | temp2) >>> 0;
            index += 0x100;
        }
    }

    return table;
}

function _hash(str, hashType) {
    var seed1 = 0x7FED7FED >>> 0;
    var seed2 = 0xEEEEEEEE >>> 0;

    str = str.toUpperCase();

    for (var i = 0; i < str.length; i++) {
        var ch = str.charCodeAt(i);

        var a = encryptionTable[(HASH_TYPES[hashType] * 0x100) + ch];
        var b = seed1 + seed2;
        seed1 = (a ^ b) >>> 0;
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }

    return seed1;
}

function _decrypt(dataBuffer, key) {
    var seed = 0xEEEEEEEE >>> 0;

    var length = dataBuffer.length / 4;
    var offset = 0;

    var result = new Buffer(dataBuffer.length);
    result.fill(' ');

    for (var i = 0; i < length; i++) {
        var encryptTableValue = encryptionTable[0x400 + (key & 0xFF)];
        seed += encryptTableValue;

        var a = dataBuffer.readUInt32LE(offset);
        var b = key + seed;
        var ch = (a ^ b) >>> 0;

        var x = (~key << 0x15) + 0x11111111;
        var y = key >>> 0x0B;
        key = (x | y) >>> 0;

        seed = ch + seed + (seed << 5) + 3;

        result.writeUInt32LE(ch, offset);

        offset += 4;
    }

    return result;
}

var encryptionTable = buildEncryptionTable();

module.exports = {
    HASH_TYPES: HASH_TYPES,
    hash: _hash,
    decrypt: _decrypt
};

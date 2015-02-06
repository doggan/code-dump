//--------------------------------------------------------------------
// Name: Shyam M Guthikonda
// Date: 04.03.05
// File: TextureManager.cpp
// Desc: Simple texture manager class.
//--------------------------------------------------------------------

#include "TextureManager.h"

#include "gl/glew.h"
#include <Windows.h>

unsigned int TextureManager::loadTexture( const char *fileName, TEX_PARAM param )
{
    std::string strFileName = fileName;
    return ( loadTexture( strFileName, param ) );
}

unsigned int TextureManager::loadTexture(
    const std::string &fileName,
    TEX_PARAM param
    )
{
    std::string fullFileName = "data\\textures\\" + fileName;
    std::string extension;

    // If this texture has alrdy been loaded, just return the ID.
    for ( unsigned int a = 0; a < m_vecTextures.size(); ++a ) {
        if ( m_vecTextures[ a ].fileName == fileName )
            return m_vecTextures[ a ].texID;
    }

    std::string::size_type pos = fileName.find( '.' );
    extension = fileName.substr( pos + 1 );

    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int pixPlanes;

    // No '.' in the file name.
    if ( pos == std::string::npos ) return 0;

    // What type of image file is this?
    if ( extension == "bmp" ) {
        imageData = loadBMP( fullFileName.c_str(), &imageWidth, &imageHeight, &pixPlanes );
    }
    else if ( extension == "ppm" ) {
        imageData = loadPNMRaw( fullFileName.c_str(), &imageWidth, &imageHeight, &pixPlanes );
    }
    else if ( extension == "tga" ) {
        imageData = loadTGA( fullFileName.c_str(), &imageWidth, &imageHeight, &pixPlanes );
    }
/*
    else if ( extension == "jpg" || extension == "jpeg" ) {
        imageData = loadJPG( fullFileName.c_str(), &imageWidth, &imageHeight, &pixPlanes );
    }*/

    else {
        // Not a supported image type.
        return 0;
    }

    if ( imageData == NULL )
        assert( !"Texture loading error!" );

    unsigned int texID;

    glGenTextures( 1, &texID );             // Generate a texture id.
    glBindTexture( GL_TEXTURE_2D, texID );  // Bind it.

    if ( param == TEX_REPEAT ) {
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    }
    else if ( param == TEX_CLAMP_TO_EDGE ) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // Set tiling options and texture filters.
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // Just use the texture for color calculations.
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // Create the texture.
    if ( pixPlanes == 3 )
        glTexImage2D( GL_TEXTURE_2D, 0, pixPlanes, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData );
    else if ( pixPlanes == 4 )
        glTexImage2D( GL_TEXTURE_2D, 0, pixPlanes, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData );

    textureInfo tex;
    tex.fileName = fileName;
    tex.texID = texID;
    tex.width = imageWidth;
    tex.height = imageHeight;
    tex.aspectRatio = (float)imageWidth / (float)imageHeight;

    m_vecTextures.push_back( tex );

    delete[] imageData;  // Free our allocated image data.

    return texID;
}

unsigned int TextureManager::loadTexture( unsigned char *imageData, unsigned int width, unsigned int height, unsigned int filter, TEX_PARAM param )
{
	if ( imageData == NULL )
		assert( !"Texture loading error!" );

	unsigned int texID;

	glGenTextures( 1, &texID );             // Generate a texture id.
	glBindTexture( GL_TEXTURE_2D, texID );  // Bind it.

	if ( param == TEX_REPEAT ) {
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	}
	else if ( param == TEX_CLAMP_TO_EDGE ) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	// Set tiling options and texture filters.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );

	// Just use the texture for color calculations.
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	// Create the texture.
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData );

	textureInfo tex;
	tex.fileName = "";
	tex.texID = texID;
	tex.width = width;
	tex.height = height;
	tex.aspectRatio = (float)width / (float)height;
	tex.data = imageData;

	m_vecTextures.push_back( tex );

	return texID;
}

void TextureManager::freeAll()
{
    unsigned int *texIDList = new unsigned int[ m_vecTextures.size() ];

    // Build a contiguous list of every texID we currently use.
    for ( unsigned int i = 0; i < m_vecTextures.size(); ++i ) {
        texIDList[ i ] = m_vecTextures[ i ].texID;
    }

    // Allow our texture ID's to be reused.
    glDeleteTextures( (GLsizei)m_vecTextures.size(), texIDList );
    m_vecTextures.clear();

    delete[] texIDList;
}

unsigned char *TextureManager::loadBMP(
    const char *fileName,
    int *xdim,
    int *ydim,
    int *pixPlanes
    )
{
    FILE *l_file;
    unsigned char *l_texture;

    BITMAPFILEHEADER fileheader; 
    BITMAPINFOHEADER infoheader;
    RGBTRIPLE rgb;

    if( (l_file = fopen( fileName, "rb"))==NULL) {
        return 0;
    }

    fread(&fileheader, sizeof(fileheader), 1, l_file);

    fseek(l_file, sizeof(fileheader), SEEK_SET);        // Jump the fileheader
    fread(&infoheader, sizeof(infoheader), 1, l_file);  //  and read the infoheader.

    // Allocate memory for: (width * height * color deep), setting it all to 0's.
    l_texture = (unsigned char *)calloc(infoheader.biWidth * infoheader.biHeight * 4, sizeof( unsigned char ));

    // Read in all the pixels.
    int j = 0;
    for ( int i = 0; i < infoheader.biWidth*infoheader.biHeight; i++)
    {            
        // Load an RGB value from the file.
        fread(&rgb, sizeof(rgb), 1, l_file); 

        // Store it.
        l_texture[j+0] = rgb.rgbtRed;   // Red component
        l_texture[j+1] = rgb.rgbtGreen; // Green component
        l_texture[j+2] = rgb.rgbtBlue;  // Blue component
        l_texture[j+3] = 255;           // Alpha value
        j += 4;                         // Go to the next position.
    }

    *xdim = infoheader.biWidth;
    *ydim = infoheader.biHeight;
    *pixPlanes = 4;

    fclose(l_file);

    return l_texture;
}

unsigned char *TextureManager::loadTGA(
    const char *filename,
    int *xdim,
    int *ydim,
    int *pixPlanes
    )
{
    FILE *file;
    unsigned char badChar;
    short int badInt;
    long imageSize;
    int	colorMode;

    STGA tgaFile;

    file = fopen(filename, "rb");

    if (!file)  return 0;

    fread(&badChar, sizeof(unsigned char), 1, file);
    fread(&badChar, sizeof(unsigned char), 1, file);

    fread(&tgaFile.imageTypeCode, sizeof(unsigned char), 1, file);

    //image type either 2 (color) or 3 (greyscale)
    if ((tgaFile.imageTypeCode != 2) && (tgaFile.imageTypeCode != 3))
    {
        fclose(file);
        return 0;
    }

    //13 bytes of useless data
    fread(&badInt, sizeof(short int), 1, file);
    fread(&badInt, sizeof(short int), 1, file);
    fread(&badChar, sizeof(unsigned char), 1, file);
    fread(&badInt, sizeof(short int), 1, file);
    fread(&badInt, sizeof(short int), 1, file);

    //image dimensions
    fread(&tgaFile.imageWidth, sizeof(short int), 1, file);
    fread(&tgaFile.imageHeight, sizeof(short int), 1, file);

    //image bit depth
    fread(&tgaFile.bitCount, sizeof(unsigned char), 1, file);

    //1 byte of garbage data
    fread(&badChar, sizeof(unsigned char), 1, file);

    //colorMode -> 3 = BGR, 4 = BGRA 
    colorMode = tgaFile.bitCount / 8;
    imageSize = tgaFile.imageWidth * tgaFile.imageHeight * colorMode;
    *pixPlanes = colorMode;

    //allocate memory for image data
    tgaFile.imageData = new unsigned char[imageSize];

    //read in image data
    fread(tgaFile.imageData, sizeof(unsigned char), imageSize, file);

    //change BGR to RGB (especially for OpenGL later on)
    for (int i = 0; i < imageSize; i += colorMode)
    {
        //swap blue and red colour value 
        tgaFile.imageData[i] ^= tgaFile.imageData[i+2] ^=
            tgaFile.imageData[i] ^= tgaFile.imageData[i+2];
    }

    //close file
    fclose(file);

    *xdim = tgaFile.imageWidth;
    *ydim = tgaFile.imageHeight;

    return tgaFile.imageData;
}

unsigned char *TextureManager::loadPNMRaw(
    const char *filename,
    int *xdim,
    int *ydim,
    int *pixPlanes
    )
{
    const char *PGMRAWMAGICNUM = "P5";
    const char *PGMASCIIMAGICNUM = "P2";
    const char *PPMRAWMAGICNUM = "P6";

    FILE *fp;
    if(!(fp = fopen(filename, "rb"))) {
        return 0;
    }

    // Read Magic Number (P5 = PGM, P6 = PPM)
    char magNum[3];
    fscanf(fp, "%s", magNum);

    if (strcmp(magNum, PGMRAWMAGICNUM) == 0) {
        *pixPlanes = 1;
    }
    else {
        if (strcmp(magNum, PPMRAWMAGICNUM) == 0) {
            *pixPlanes = 3;
        }
        else {
            printf("ERROR(readPNMRaw): Bad magic number. Is the image file PPM or PGM rawbits?\n");
            exit(0);
        }
    }

    unsigned int maxPixVal;
    // Read image dimensions and pixel size
    fscanf(fp, "%d %d\n", xdim, ydim);
    fscanf(fp, "%d\n", &maxPixVal);

    // Compute the size in bytes of the image data
    unsigned long numBytes = *xdim * *ydim * *pixPlanes;

    // Allocate volume buffer
    unsigned char *imgArr = new unsigned char[numBytes];

    unsigned long bytesRead = fread(imgArr, sizeof(unsigned char), numBytes, fp);

    if (bytesRead != numBytes) {
        printf("ERROR(readPNMRaw): read %d bytes, but expected %d bytes ", 
            bytesRead, numBytes);
        printf("when reading file '%s'\n", filename);
        exit(0);
    }

    fclose(fp);
    return imgArr;
}

/*
unsigned char *TextureManager::loadJPG(
    const char *filename,
    int *xdim,
    int *ydim,
    int *pixPlanes
    )
{
    FILE *fp;
    if(!(fp = fopen(filename, "rb"))) {
        return 0;
    }

    // Read Magic Number (P5 = PGM, P6 = PPM)
    char magNum[3];
    fscanf(fp, "%s", magNum);

    if (strcmp(magNum, PGMRAWMAGICNUM) == 0) {
        *pixPlanes = 1;
    }
    else {
        if (strcmp(magNum, PPMRAWMAGICNUM) == 0) {
            *pixPlanes = 3;
        }
        else {
            printf("ERROR(readPNMRaw): Bad magic number. Is the image file PPM or PGM rawbits?\n");
            exit(0);
        }
    }

    unsigned int maxPixVal;
    // Read image dimensions and pixel size
    fscanf(fp, "%d %d\n", xdim, ydim);
    fscanf(fp, "%d\n", &maxPixVal);

    // Compute the size in bytes of the image data
    unsigned long numBytes = *xdim * *ydim * *pixPlanes;

    // Allocate volume buffer
    unsigned char *imgArr = dbgNew unsigned char[numBytes];

    unsigned long bytesRead = fread(imgArr, sizeof(unsigned char), numBytes, fp);

    if (bytesRead != numBytes) {
        printf("ERROR(readPNMRaw): read %d bytes, but expected %d bytes ", 
            bytesRead, numBytes);
        printf("when reading file '%s'\n", filename);
        exit(0);
    }

    fclose(fp);
    return imgArr;
}*/

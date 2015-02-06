//--------------------------------------------------------------------
// Name: Shyam M Guthikonda
// Date: 04.03.05
// File: TextureManager.h
// Desc: Simple texture manager class.
//--------------------------------------------------------------------

#ifndef _CTEXTUREMANAGER_H_
#define _CTEXTUREMANAGER_H_

#include "Singleton.h"

#include <string>
#include <vector>

class TextureManager : public Singleton< TextureManager >
{
public:
    enum TEX_PARAM {
        TEX_REPEAT,
        TEX_CLAMP_TO_EDGE
    };

    TextureManager() {}
    virtual ~TextureManager() { freeAll(); }

    /* Takes the file name of texture, loads it using OpenGL, and returns a unique
       texture ID. This function will return 0 if an error occurs. The char * version
       simply constructs a string and calls the string version, as strings are more
       robust.
    */
    unsigned int loadTexture( const char *fileName, TEX_PARAM param = TEX_REPEAT );
    unsigned int loadTexture( const std::string &fileName, TEX_PARAM param = TEX_REPEAT );

	// TODO: this should be cleaned up.. merged with the previous functions. They need to be
	//			cleaned up too, heh.
	/* Given procedurally generated image data, give me a texture id!
	*/
	unsigned int loadTexture( unsigned char *imageData, unsigned int width, unsigned int height, unsigned int filter, TEX_PARAM param = TEX_REPEAT );

    /* Frees all generated texture ID's.
    */
    void freeAll();

    /* Simple struct to store information about each texture.
    */
    struct textureInfo {
        std::string fileName;
        unsigned int texID;
        unsigned int width, height;
        float aspectRatio;
		unsigned char *data;	// This may be NULL depending on which loadTexture() function is used...
    };

    /* Looks in our m_vecTextures to find the given ID. Returns a reference to this
       entry.
    */
    textureInfo &findTex( unsigned int id ) {
        for ( unsigned int i = 0; i < m_vecTextures.size(); ++i ) {
            if ( m_vecTextures[ i ].texID == id )
                return m_vecTextures[ i ];
        }
        assert( !"Texture ID Doesn't Exist!" );
    }

private:
    struct STGA
    { 
        STGA()
        {
            imageData = (unsigned char*)0;
            imageWidth = 0;
            imageHeight = 0;
            imageTypeCode = 0;
            bitCount = 0;
        }

        int imageWidth;
        int imageHeight;
        unsigned char imageTypeCode;
        unsigned char bitCount;
        unsigned char *imageData;
    };

    std::vector< textureInfo > m_vecTextures;       // Info for every texture we load.

    /* Returns the loaded raw pgm/ppm file image file data. Author = Naeem Shareef.
    */
    unsigned char *loadPNMRaw(
        const char *filename,
        int *xdim,
        int *ydim,
        int *pixPlanes     // # of color channels per pixel. (1 = grey, 3 = rgb, 4 = rgba).
        );

    unsigned char *loadBMP(
        const char *fileName,
        int *xdim,
        int *ydim,
        int *pixPlanes
        );

    unsigned char *loadJPG(
        const char *fileName,
        int *xdim,
        int *ydim,
        int *pixPlanes
        );

    unsigned char *loadTGA(
        const char *filename,
        int *xdim,
        int *ydim,
        int *pixPlanes
        );
};

#endif // _CTEXTUREMANAGER_H_
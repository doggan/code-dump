/*
- This class needs to keep track of every texture that has been loaded.

- Will will query for a Texture. If the texture has been loaded (into OpenGL),
we return just the textureID. If it hasn't been loaded, load it, then return
the id.

- I think it may be managing Images instead of textures. Maybe change the name of
this class/file.

*/

#ifndef _ImageManager_h_
#define _ImageManager_h_

#include "Singleton.h"
#include "Common.h"

#include <string>
#include <vector>

class ImageManager : public Singleton< ImageManager >
{
public:
	ImageManager();
	~ImageManager();

	/* Loads the given image from fileName, storing the result in img, which
	* can later be used to render the texture by OpenGL.
	*/
	void getImage(const std::string &fileName, Image *&img);

	// TODO: unloadAll (for level changes).

private:
	Image *findImage(const std::string &name);

	std::vector< Image* > m_loadedImages;
};

#endif // _ImageManager_h_
#ifndef _Renderer_h_
#define _Renderer_h_

#include "Common.h"

#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"

#include <string>

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {}

	/* Loads the given image from fileName, storing the result in img, which
	* can later be used to render the texture by OpenGL.
	*/
	void loadImage(const std::string &fileName, Image *img);

	// TODO: create an unloadImage to free the texture id.

	void draw(Image *img, int x, int y);
	// TODO: possibly need a drawBackground AND drawEntity
};

#endif // _Renderer_h_
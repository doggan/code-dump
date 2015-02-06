#include "Renderer.h"

#include <cassert>

Renderer::Renderer()
{
	// OpenGL initialization.
	glClearColor(0, 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
}

void Renderer::loadImage(const std::string &fileName, Image *img)
{
	SDL_Surface *SDLsurface = IMG_Load(fileName.c_str());
	
	GLuint textureID;
	SDL_Surface *OGLsurface;

	OGLsurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		SDLsurface->w, SDLsurface->h,
		32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
		0x000000FF,
		0x0000FF00,
		0x00FF0000,
		0xFF000000
#else
		0xFF000000,
		0x00FF0000,
		0x0000FF00,
		0x000000FF
#endif
		);

	assert(OGLsurface);

	// Copy the surface into the GL texture image.
	SDL_Rect area;
	area.x = 0;
	area.y = 0;
	area.w = SDLsurface->w;
	area.h = SDLsurface->h;
	SDL_BlitSurface(SDLsurface, &area, OGLsurface, &area);

	Uint32 numPixels = OGLsurface->w * OGLsurface->h;
	for (Uint32 i = 0; i < numPixels; ++i)
	{
		Uint32 index = i * 4;

//		Uint8 r = static_cast<Uint8*>(OGLsurface->pixels)[index];
		Uint8 g = static_cast<Uint8*>(OGLsurface->pixels)[index + 1];
		Uint8 b = static_cast<Uint8*>(OGLsurface->pixels)[index + 2];
		Uint8 &a = static_cast<Uint8*>(OGLsurface->pixels)[index + 3];

		// Cyan goes to full transparency.
		if (g == 255 && b == 255)
			a = 0;
	}

	// Create an OpenGL texture for the image.
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		OGLsurface->w, OGLsurface->h,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		OGLsurface->pixels);

	// Write out.
	img->id = textureID;
	img->w = OGLsurface->w;
	img->h = OGLsurface->h;

	// Cleanup.
	SDL_FreeSurface(OGLsurface);
	SDL_FreeSurface(SDLsurface);
}

void Renderer::draw(Image *img, int x, int y)
{
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBindTexture(GL_TEXTURE_2D, img->id);

	glTranslatef((float)x, (float)y, 0 );
	glScalef(img->scale, img->scale, img->scale);

	int left = -img->w / 2;
	int right = img->w / 2;
	int top = -img->h;
	int bottom = 0;

	glBegin( GL_QUADS );
		glTexCoord2f(0, 0);
		glVertex2i(left, top);
		glTexCoord2f(1, 0);
		glVertex2i(right, top);
		glTexCoord2f(1, 1);
		glVertex2i(right, bottom);
		glTexCoord2f(0, 1);
		glVertex2i(left, bottom);
	glEnd();

	glPopAttrib();
	glPopMatrix();
}
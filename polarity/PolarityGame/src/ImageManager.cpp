#include "ImageManager.h"

#include "SDL_image.h"
#include "SDL_opengl.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
	for (uint i = 0; i < m_loadedImages.size(); ++i)
	{
		delete m_loadedImages[i];
	}
}

void ImageManager::getImage(const std::string &fileName, Image *&img)
{
	// Don't re-load the image if it's already loaded!
	Image *existing = findImage(fileName);
	if (existing)
	{
		img = existing;
		return;
	}

	img = new Image;

	SDL_Surface *SDLsurface = IMG_Load(std::string("data/images/" + fileName).c_str());

	if (SDLsurface == NULL)
		EXCEPTION(Exception::ERR_FILE_NOT_FOUND, "getImage()", "Error loading image: " + fileName);

	SDL_Surface *OGLsurface;

	OGLsurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		SDLsurface->w, SDLsurface->h,
		SCREEN_BPP,
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

	if (OGLsurface == NULL)
		EXCEPTION(Exception::ERR_INTERNAL_ERROR, "getImage()", "RGB surface creation error");

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

		Uint8 r = static_cast<Uint8*>(OGLsurface->pixels)[index];
		Uint8 g = static_cast<Uint8*>(OGLsurface->pixels)[index + 1];
		Uint8 b = static_cast<Uint8*>(OGLsurface->pixels)[index + 2];
		Uint8 &a = static_cast<Uint8*>(OGLsurface->pixels)[index + 3];

		// Cyan goes to full transparency.
		if (r == 0 && g == 255 && b == 255)
			a = 0;
	}

	// Create an OpenGL texture for the image.
	glGenTextures(1, &static_cast<GLuint>(img->id));
	glBindTexture(GL_TEXTURE_2D, img->id);
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
	img->w = OGLsurface->w;
	img->h = OGLsurface->h;
	img->name = fileName;

	// Cleanup.
	SDL_FreeSurface(OGLsurface);
	SDL_FreeSurface(SDLsurface);

	m_loadedImages.push_back(img);
}

Image *ImageManager::findImage(const std::string &name)
{
	for (uint i = 0; i < m_loadedImages.size(); ++i)
	{
		if (m_loadedImages[i]->name == name)
			return m_loadedImages[i];
	}

	return NULL;
}
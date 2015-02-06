#include "Renderer.h"

#include "SDL_opengl.h"

Renderer::Renderer()
{
	// OpenGL initialization.
	glClearColor(0, 0, 0, 0);

	GLint view[4];
	glGetIntegerv(GL_VIEWPORT, view);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(view[0], view[0] + view[2], view[1] + view[3], view[1], -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
}

void Renderer::blit(const Image *img, int x, int y)
{
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, img->id);

	glTranslatef((float)x, (float)y, 0 );

	int left = 0;
	int right = img->w;
	int top = 0;
	int bottom = img->h;

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

	glPopMatrix();
}
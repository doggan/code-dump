#include "2D_Rendering.h"

#include <cmath>

#include <gl/glut.h>

namespace _2D
{
	void hollowTriangle(float w, float h)
	{
		float w2 = w / 2;
		float h2 = h / 2;

		glBegin(GL_LINE_LOOP);
			glVertex2f(-w2, -h2);
			glVertex2f(-w2, h2);
			glVertex2f(w2, 0);
		glEnd();
	}

	void solidTriangle(float w, float h)
	{
		float w2 = w / 2;
		float h2 = h / 2;

		glBegin(GL_TRIANGLES);
			glVertex2f(-w2, -h2);
			glVertex2f(-w2, h2);
			glVertex2f(w2, 0);
		glEnd();
	}

	void hollowCircle(float r)
	{
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 360; ++i)
		{
			float degInRad = i * PI / 180.0f;
			glVertex2f(cos(degInRad) * r, sin(degInRad) * r);
		}
		glEnd();
	}

	void renderBitmapString(float x, float y, void *font, std::string str)
	{  
		glRasterPos2f(x, y);
		for (unsigned i = 0; i < str.length(); ++i)
		{
			glutBitmapCharacter(font, str[i]);
		}
	}
}
#ifndef _2D_Rendering_h_
#define _2D_Rendering_h_

#include <string>

#define PI 3.14159265f

namespace _2D
{
	void hollowTriangle(float w, float h);
	void solidTriangle(float w, float h);

	void hollowCircle(float r);

	void renderBitmapString(float x, float y, void *font, std::string str);
}

#endif // _2D_Rendering_h_
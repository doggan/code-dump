#ifndef _Renderer_h_
#define _Renderer_h_

#include "Common.h"
#include "Singleton.h"

#include <string>

class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer() {}

	void blit(const Image *img, int x, int y);
};

#endif // _Renderer_h_
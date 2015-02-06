#ifndef _Common_h_
#define _Common_h_

#include "SDL.h"

#include <string>

// Change to const variables.
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32

typedef Uint32 uint;

struct Image
{
	std::string name;
	uint id;
	int w, h;	// TODO: may be able to eliminate this.. save some memory
};

enum POLARITY
{
	DARK,
	LIGHT
};

enum TILE_LAYER
{
	LAYER_COLLISION = 0,
	LAYER_2,
	LAYER_1,
	LAYER_0
};

struct Rect
{
	uint x, y;
	uint w, h;
};

struct Stats
{
	Stats()
	{
		level = hp = energy = str = stam = intel = lResist = dResist = 0;
	}

	Stats operator+(const Stats &x) const
	{
		Stats s(*this);
		s.level += x.level;
		s.hp += x.hp;
		s.energy += x.energy;
		s.def += x.def;
		s.str += x.str;
		s.stam += x.stam;
		s.intel += x.intel;
		s.lResist += x.lResist;
		s.dResist += x.dResist;
		return s;
	}

	Stats operator-(const Stats &x) const
	{
		Stats s(*this);
		s.level -= x.level;
		s.hp -= x.hp;
		s.energy -= x.energy;
		s.def -= x.def;
		s.str -= x.str;
		s.stam -= x.stam;
		s.intel -= x.intel;
		s.lResist -= x.lResist;
		s.dResist -= x.dResist;
		return s;
	}

	uint level;
	int hp;
	int energy;
	int def;
	int str, stam, intel;
	int lResist, dResist;
};

#include "Exception.h"

#endif // _Common_h_
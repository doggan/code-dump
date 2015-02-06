#ifndef _InventoryDisplay_h_
#define _InventoryDisplay_h_

#include "Common.h"

#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "guichan/opengl.hpp"
#include "guichan/opengl/openglsdlimageloader.hpp"

// Forward declarations.
class Inventory;

class InventoryDisplay
{
public:
	InventoryDisplay(gcn::Container *top, gcn::ImageFont *f);
	~InventoryDisplay();
	
	void setInventory(const Inventory *i, const Stats &stats);
	
	void toggle();
	
private:
	gcn::Label *m_health;
	gcn::Label *m_energy;
};

#endif // _InventoryDisplay_h_
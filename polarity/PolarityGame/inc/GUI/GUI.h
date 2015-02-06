#ifndef _GUI_h_
#define _GUI_h_

#include "Common.h"
#include "Singleton.h"

#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "guichan/opengl.hpp"
#include "guichan/opengl/openglsdlimageloader.hpp"

// Forward declarations.
class ChatDisplay;
class Inventory;
class InventoryDisplay;
class PlayerInfoDisplay;

class GUI : public Singleton< GUI >
{
public:
	GUI();
	~GUI();

	void update();
	
	void addToChat(const std::string &txt);

	void pushInput(const SDL_Event &e);

	void setFPS(int val);
	void setInventory(const Inventory *i, const Stats &stats);
	void setPlayerInfo(const Stats &stats, uint xp, int lEnergy, int dEnergy);
	
	void toggleInventory();

private:
	gcn::SDLInput *m_input;
	gcn::OpenGLSDLImageLoader *m_imageLoader;
	gcn::OpenGLGraphics *m_graphics;

	gcn::Gui *m_gui;
	gcn::Container *m_top;
	
	ChatDisplay *m_chatDisplay;
	InventoryDisplay *m_inventoryDisplay;
	PlayerInfoDisplay *m_playerInfoDisplay;
	gcn::Label *m_fps;
	
	// TODO: design a font manager
	gcn::ImageFont *m_font1;
	gcn::ImageFont *m_font2;
};

#endif // _GUI_h_
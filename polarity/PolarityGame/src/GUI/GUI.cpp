#include "GUI.h"

#include "Application.h"
#include "ChatDisplay.h"
#include "InventoryDisplay.h"
#include "PlayerInfoDisplay.h"
#include "Utilities.h"

GUI::GUI()
{
	m_imageLoader = new gcn::OpenGLSDLImageLoader();
	gcn::Image::setImageLoader(m_imageLoader);
	m_graphics = new gcn::OpenGLGraphics(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_input = new gcn::SDLInput();

	m_font1 = new gcn::ImageFont("data/images/fonts/fixedfont.png",
		" abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ01234567"
		"89:@!\"$%&/=?^+*#[]{}()<>_;'.,\\|-~`"
		"øåáÁéÉíÍóÓúÚçë¥£¢¡¿àãõêñÑöüäÖÜÄßèÈÅ");
	
	m_font2 = new gcn::ImageFont("data/images/fonts/rpgfont_wider.png",
		" abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789.,!?-+/():;%&`'*#=[]"
		"\"(){}^~|_@$\\"
		"áÁéÉíÍóÓúÚçë¥£¢¡¿àãõêñÑöüäÖÜÄßøèÈåÅ");
		
	gcn::Widget::setGlobalFont(m_font1);

	m_gui = new gcn::Gui();
	m_gui->setGraphics(m_graphics);
	m_gui->setInput(m_input);

	m_top = new gcn::Container();    
	m_top->setDimension(gcn::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	m_top->setOpaque(false);
	m_gui->setTop(m_top);
	
	m_fps = new gcn::Label("FPS: -");
	m_fps->setPosition(SCREEN_WIDTH - 60, 0);
	m_fps->setSize(60, 20);
	m_fps->setFont(m_font2);
	m_top->add(m_fps);
	
	m_chatDisplay = new ChatDisplay(m_top);
	m_inventoryDisplay = new InventoryDisplay(m_top, m_font2);
	m_playerInfoDisplay = new PlayerInfoDisplay(m_top, m_font2);
}

GUI::~GUI()
{
	delete m_chatDisplay;
	delete m_inventoryDisplay;
	delete m_playerInfoDisplay;
	
	delete m_font1;
	delete m_font2;
	
	delete m_top;
	delete m_gui;

	delete m_input;  
	delete m_graphics;
	delete m_imageLoader;
}

void GUI::addToChat(const std::string &txt)
{
	m_chatDisplay->addRow(txt);
}

void GUI::pushInput(const SDL_Event &event)
{
	m_input->pushInput(event);
}

void GUI::setInventory(const Inventory *i, const Stats &stats)
{
	m_inventoryDisplay->setInventory(i, stats);
}

void GUI::setFPS(int val)
{
	m_fps->setCaption("FPS: " + toStr(val));
}

void GUI::setPlayerInfo(const Stats &stats, uint xp, int lEnergy, int dEnergy)
{
	m_playerInfoDisplay->setPlayerInfo(stats, xp, lEnergy, dEnergy);
}

void GUI::update()
{
	m_gui->logic();
	m_gui->draw();
}

void GUI::toggleInventory()
{
	m_inventoryDisplay->toggle();
}
#ifndef _Application_h_
#define _Application_h_

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

#include "Singleton.h"
#include "MtxLib.h"

#include <string>
#include <list>

// Forward declarations.
class Player;
class NPC;

class Application : public Singleton<Application>
{
public:
	Application();
	virtual ~Application();

	void display();
	void idle();
	void reshape(int w, int h);

	void processNormalKeys(unsigned char key, int x, int y);
	void processSpecialKeys(int key, int x, int y);

	void processMouse(int button, int state, int x, int y);
	void processMouseActiveMotion(int x, int y);

	Player *getPlayer() { return m_player; }
	std::list< NPC* > getNPCs() { return m_npcs; }

private:
	void displayFPS();

	Player *m_player;

	std::list< NPC* > m_npcs;
};

#endif // _Application_h_
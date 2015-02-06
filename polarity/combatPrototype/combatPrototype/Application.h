#ifndef _Application_h_
#define _Application_h_

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_opengl.h"

#include "Singleton.h"
#include "EntityBase.h"
#include "World.h"

#include <vector>

// TODO: implement exception system instead of checking for bools for all initialization stuff.

class Input;
class Renderer;
class World;

class Application : public Singleton<Application>
{
public:
	Application();
	virtual ~Application();

	bool init();

	void run();

	World *getCurrentWorld() { return m_worlds[0]; }

private:
	bool handleInput();
	EntityBase *handleTargeting(int x, int y, const std::vector<EntityBase*> &entityList);

	void drawScene();

	bool loadWorlds();
	bool loadPlayer();

	// TODO: need better system for fonts. Let's make a font manager?
	TTF_Font *m_font;

	enum GAME_MODE
	{
		INTRO_SCREEN,
		CREDITS,
		OPTIONS,
		GAME
	};

	GAME_MODE m_currentMode;

	// TODO: should eventually take as parameters:
	//	PlayerList (all the players who entered combat)
	//  EnemyList (all the enemies who entered combat)
	//  BattlefieldType (the background image of the combat area)
	void enterCombat(Player *p, NPC *npc);

	// TODO: how to implement body?
	void resurrectToBind();

	void move(World::DIRECTION dir);

	Renderer *m_renderer;
	Input *m_input;

	Player *m_player;
	std::vector< NPC* > m_npcList;

	std::vector< World* > m_worlds;
};

#endif // _Application_h_
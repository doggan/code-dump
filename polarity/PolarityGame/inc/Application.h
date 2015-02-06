#ifndef _Application_h_
#define _Application_h_

#include "GestureSystem.h"
#include "Singleton.h"

#include "SDL.h"
#include "SDL_opengl.h"

#include <vector>

// Forward declarations.
class EntityBase;
class NPC;
class Player;
class Timer;
class World;

class Application : public Singleton<Application>
{
public:
	Application();
	~Application();

	void init();

	void run();

	World *getCurrentWorld() { return m_worlds[0]; }

	std::vector< EntityBase* > getCurrentEntities();

private:
	void preFrame(float deltaTime);
	void frame(float deltaTime);

	void handleInput(float deltaTime);

	// Return true if something was targeted. False if not.
	bool handleTargeting(const vec2<int> &mousePos);

	void drawScene();
	void drawCollisionBox();
	void drawTargetBox();
	void drawSpellGesture();

	void loadData();

	std::vector<GestureSystem::Point2D> m_currentGesture;

	enum GAME_MODE
	{
		INTRO_SCREEN,
		CREDITS,
		OPTIONS,
		GAME
	};

	GAME_MODE m_currentMode;

	Timer *m_timer;

	// TODO: !
public:
	Player *m_player;

private:
	std::vector< NPC* > m_npcList;		// TODO: this shouldn't be here.

	std::vector< World* > m_worlds;		// // TODO: this shouldn't be here. Make a world manager.
};

#endif // _Application_h_
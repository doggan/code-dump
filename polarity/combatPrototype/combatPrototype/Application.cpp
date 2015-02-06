#include "Application.h"

#include "Common.h"
#include "GlobalTimer.h"
#include "Input.h"
#include "Renderer.h"
#include "Utilities.h"
#include "SpellSystem.h"
#include "TypingSystem.h"

#include "tinyxml/tinyxml.h"

#include <time.h>

Application::Application()
{
	// Seed the PRNG.
	srand(static_cast<Uint32>(time(NULL)));
	
	// TODO:
	m_currentMode = GAME;

	new GlobalTimer(500);
	GlobalTimer::getSingleton().start();
	
	new SpellSystem();
	new TypingSystem();

	// TODO: initialize all vars to NULL
	m_player = NULL;
}

Application::~Application()
{
	// Clean up worlds.
	for (Uint16 i = 0; i < m_worlds.size(); ++i)
		delete m_worlds[i];

	GlobalTimer::getSingleton().stop();
	delete GlobalTimer::getSingletonPtr();

	delete TypingSystem::getSingletonPtr();
	delete SpellSystem::getSingletonPtr();

	SDL_Quit();

	delete m_renderer;
	delete m_input;

	TTF_CloseFont(m_font);
	SDL_EnableUNICODE( SDL_DISABLE ); 
}

bool Application::init()
{
	// Initialize all SDL subsystems.
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;    

	// Set up the screen.
	SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_DOUBLEBUF);

	if (!screen)
		return false;

	// Set the window caption.
	SDL_WM_SetCaption("Combat Prototype", NULL);

	// Initialize the various modules.
	m_renderer = new Renderer();
	m_input = new Input();

	// Font initialization (TODO: let's make a font manager).
	if (TTF_Init() == -1)
		return false;

	m_font = TTF_OpenFont("arial.ttf", 28);

	SDL_EnableUNICODE( SDL_ENABLE );

	if (!m_font)
		return false;

	if (!loadPlayer())
		return false;

	if (!loadWorlds())
		return false;

	return true;
}

bool Application::loadWorlds()
{
	// TODO: need an xml file with list of all worlds to load (each world is it's own xml file).
	// TODO: eventually we'd only need to load the current world? But if we could load everything at once it might be nicer...

	World *w = new World(m_renderer, m_player);
	if (!w->load("data/worlds.xml"))
		return false;

	m_worlds.push_back(w);

	return true;
}

bool Application::loadPlayer()
{
	// Player.
	m_player = new Player();
	m_renderer->loadImage("player.png", m_player->m_image);
	m_player->m_tileX = 0; m_player->m_tileY = 0;
	m_player->m_locX = m_player->m_newLocX = 58;
	m_player->m_locY = m_player->m_newLocY = 211;
	m_player->m_currentState = EntityBase::WORLD;

	return true;
}

void Application::run()
{
	bool running = true;

	// Main game loop.
	while (running == true)
	{
		glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

		glLoadIdentity();

		GlobalTimer::getSingleton().update();

		running = handleInput();

		// Update all active entities.
		std::vector< EntityBase* > activeEntities = m_worlds[0]->getCurrentEntities();
		for (Uint16 i = 0; i < activeEntities.size(); ++i)
			activeEntities[i]->update();

		drawScene();

		// TODO: this will all eventually be moved to some sort of GUI system
//		{
//			SDL_Color red;
//			red.r = 255; red.g = 0; red.b = 0;
//			SDL_Color blue;
//			blue.r = 0; blue.g = 0; blue.b = 255;
//
//			std::string playerHP = intToString(m_player->m_stats.hp);
//			SDL_Surface *pHP = TTF_RenderText_Solid(m_font, playerHP.c_str(), red);
//			m_renderer->draw(pHP, 30, 50);
//
//			std::string playerMP = intToString(m_player->m_stats.mp);
//			SDL_Surface *pMP = TTF_RenderText_Solid(m_font, playerMP.c_str(), blue);
//			m_renderer->draw(pMP, 30, 75);
//
//			m_renderer->unloadImage(pHP);
//			m_renderer->unloadImage(pMP);
//
//			if (activeEntities.size() > 1)
//			{
//				std::string npcHP = intToString(activeEntities[1]->m_stats.hp);
//				SDL_Surface *nHP = TTF_RenderText_Solid(m_font, npcHP.c_str(), red);
//				m_renderer->draw(nHP, 450, 250);
//
//				std::string npcMP = intToString(activeEntities[1]->m_stats.mp);
//				SDL_Surface *nMP = TTF_RenderText_Solid(m_font, npcMP.c_str(), blue);
//				m_renderer->draw(nMP, 450, 275);
//
//				m_renderer->unloadImage(nHP);
//				m_renderer->unloadImage(nMP);
//			}
//		}

		SDL_GL_SwapBuffers();
	}
}

void Application::drawScene()
{
	switch (m_currentMode)
	{
	case GAME:
		{
			switch (m_player->m_currentState)
			{
			// Draw world screen.
			case EntityBase::WORLD:
				{
					m_worlds[0]->renderWorld();
				}
				break;

			// Draw combat screen.
			case EntityBase::COMBAT:
				{
					m_worlds[0]->renderCombat();
				}
				break;

			case EntityBase::DEAD:
				// TODO: render 'dead' screen (red tint shader, etc).
				break;
			}
		}
	}
}

void Application::enterCombat(Player *p, NPC *npc)
{
	printf("Entering combat...\n");

	p->m_currentState = EntityBase::COMBAT;
	npc->m_currentState = EntityBase::COMBAT;

	std::vector<EntityBase*> combatants;
	combatants.push_back(p);
	npc->setCombatantList(combatants);
}

bool Application::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN)
		{
			TypingSystem::getSingleton().update(event.key.keysym.unicode);

			if (( event.key.keysym.unicode >= (Uint16)'1' ) &&
				( event.key.keysym.unicode <= (Uint16)'9' ))
			{
				char temp = (char)event.key.keysym.unicode;
				m_player->cast(atoi(&temp));
			}
			else if (event.key.keysym.sym == SDLK_F1)
				resurrectToBind();
			else if (event.key.keysym.sym == SDLK_UP)
				move(World::NORTH);
			else if (event.key.keysym.sym == SDLK_DOWN)
				move(World::SOUTH);
			else if (event.key.keysym.sym == SDLK_LEFT)
				move(World::WEST);
			else if (event.key.keysym.sym == SDLK_RIGHT)
				move(World::EAST);
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				return false;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int x = event.button.x;
				int y = event.button.y;

				printf("%i, %i\n", x, y);

				m_player->m_currentTarget = handleTargeting(x, y, m_worlds[0]->getCurrentEntities());

				if (m_player->m_currentTarget)
					printf("%s\n", m_player->m_currentTarget->m_name.c_str());
				else
				{
					// Movement within a tile.
					m_player->m_newLocX = (float)x;
					m_player->m_newLocY = (float)y;
				}
			}
			else if (event.button.button == SDL_BUTTON_RIGHT)
			{
				// Enter combat!
				if (dynamic_cast<NPC*>(m_player->m_currentTarget) &&
					m_player != m_player->m_currentTarget &&
					m_player->m_currentState == EntityBase::WORLD)
				{
					if (m_worlds[0]->isCombatAllowed())
						enterCombat(m_player, dynamic_cast<NPC*>(m_player->m_currentTarget));
					else
						printf("Combat not allowed in this area!\n");
				}
			}
		}
		else if (event.type == SDL_QUIT)
			return false;
	}

	return true;
}

void Application::move(World::DIRECTION dir)
{
	if (m_worlds[0]->isMoveAllowed(dir))
	{
		switch (dir)
		{
		case World::NORTH:
			m_player->m_tileX -= 1;
			break;

		case World::SOUTH:
			m_player->m_tileX += 1;
			break;

		case World::EAST:
			m_player->m_tileY += 1;
			break;

		case World::WEST:
			m_player->m_tileY -= 1;
			break;
		}
	}
}

EntityBase *Application::handleTargeting(int x, int y, const std::vector<EntityBase*> &entityList)
{
	// Targeting system.
	// TODO: - in the future, we may have issues with overlapping objects. We will
	//			want to select the object "closer to the camera". I think we can
	//			just look at the y coordinate, and take the object with the largest y
	//			(lower on the screen, which means closer to the camera using the perspective).
	//		 - Will also need to handle the thing where objects away from the camera
	//		    have their rectangles scaled down in size to simulate perspective.
	for (Uint16 i = 0; i < entityList.size(); ++i)
	{
//		const int 
//		const SDL_Rect &rect = entityList[i]->getLoc();
//
//		// Within bounds of the rectangle!
//		if (x > rect.x &&
//			x < rect.x + rect.w &&
//			y > rect.y &&
//			y < rect.y + rect.h)
//		{
//			return entityList[i];
//		}
	}

	return NULL;
}

void Application::resurrectToBind()
{
	if (m_player->m_currentState == EntityBase::DEAD)
	{
		m_player->m_currentState = EntityBase::WORLD;

		// TODO: set player to his 'bound location tile'.
		m_player->m_tileX = 0;
		m_player->m_tileY = 0;

		// TODO: somehow do body/spirit thing (see wiki).
		// TODO: current 'bug' in that the NPC is still hostile towards player
		//		after player is dead, releases spirit, then comes back to NPC screen.
	}
}
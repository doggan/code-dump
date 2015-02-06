#include "Application.h"

#include "Common.h"
#include "GUI.h"
#include "ImageManager.h"
#include "Input.h"
#include "ItemManager.h"
#include "NPCManager.h"
#include "Player.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Renderer.h"
#include "SpellSystem.h"
#include "Timer.h"
#include "Utilities.h"
#include "World.h"

// TODO: remove
#include "Inventory.h"

#include "tinyxml.h"

#include <ctime>
#include <cmath>

Application::Application()
{
	// Seed the PRNG.
	srand(static_cast<Uint32>(time(NULL)));

	// TODO:
	m_currentMode = GAME;

	// TODO: initialize all vars to NULL
	m_player = NULL;
}

Application::~Application()
{
	// Game components.
	{
		delete SpellSystem::getSingletonPtr();
		delete ItemManager::getSingletonPtr();
		delete NPCManager::getSingletonPtr();
		delete ProjectileManager::getSingletonPtr();

		// Clean up worlds.
		for (Uint16 i = 0; i < m_worlds.size(); ++i)
			delete m_worlds[i];
	}

	// Core components.
	{
		delete m_timer;
		delete Input::getSingletonPtr();
		delete GUI::getSingletonPtr();
		delete Renderer::getSingletonPtr();
		delete ImageManager::getSingletonPtr();

		SDL_Quit(); 
	}
}

void Application::init()
{
	// Core components.
	{
		// Initialize all SDL subsystems.
		if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) == -1)
			EXCEPTION(Exception::ERR_INTERNAL_ERROR, "init()", "SDL initialization failure");

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		
		// Enable v-sync to get rid of tearing.
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

		// Set up the screen.
		SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL | SDL_DOUBLEBUF);
		if (!screen)
			EXCEPTION(Exception::ERR_INTERNAL_ERROR, "init()", "SDL video mode initialization failure");

		// Set the window caption.
		SDL_WM_SetCaption("Combat Prototype", NULL);

		// Initialize the various modules.
		new ImageManager();
		new Renderer();
		new GUI();
		new Input();
		m_timer = new Timer();

		SDL_EnableUNICODE(SDL_ENABLE);
		SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	}

	// Game components.
	{
		new SpellSystem();
		new ItemManager();
		new NPCManager();
		new ProjectileManager();

		loadData();
	}
}

void Application::loadData()
{
	ItemManager::getSingleton().loadItems("data/items.xml");
	NPCManager::getSingleton().loadNPCs("data/npcs.xml");

	m_player = new Player;
	m_player->load("data/player.xml");

	// TODO: Only need to load the current world. Do this in WorldManager!
	World *w = new World;
	w->load("data/world01.xml");
	m_worlds.push_back(w);
}

void Application::run()
{
	bool running = true;

	m_timer->start();

	// Main game loop.
	while (running == true)
	{
		m_timer->update();

		running = Input::getSingleton().update();
		if (!running)
			break;

		float deltaTime = m_timer->getTimeElapsed();

		// Update FPS to screen.
		GUI::getSingleton().setFPS(m_timer->getFrameRate());

		preFrame(deltaTime);

		vec2<float> playerPos = m_player->getPos();
		m_worlds[0]->setRenderArea(
			vec2<float>(playerPos.x - SCREEN_WIDTH / 2, playerPos.y - SCREEN_HEIGHT / 2),
			vec2<uint>(SCREEN_WIDTH, SCREEN_HEIGHT)
			);

		frame(deltaTime);
	}

	m_timer->stop();
}

void Application::preFrame(float deltaTime)
{
	handleInput(deltaTime);
	
	// Update all projectiles.
	ProjectileManager::getSingleton().update(deltaTime);

	// Update all active entities.
	std::vector< EntityBase* > activeEntities = getCurrentEntities();
	for (uint i = 0; i < activeEntities.size(); ++i)
		activeEntities[i]->update(deltaTime);

	SpellSystem::getSingleton().update(deltaTime);
}

void Application::frame(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	drawScene();
	GUI::getSingleton().update();

	SDL_GL_SwapBuffers();
}

void Application::handleInput(float deltaTime)
{
	// TODO: temporary hotkey :)
	if (Input::getSingleton().isKeyDown(SDLK_F1))
	{
		printf("Saving player...\n");
		m_player->save("data/player1.xml");
		printf("Save complete!\n");
	}

	const float moveSpeed = m_player->getSpeed() * deltaTime;
	const vec2<float> pos = m_player->getPos();
	vec2<float> posOffset(0, 0);

	// Up.
	if (Input::getSingleton().isKeyHeld(SDLK_w))
	{
		Rect cb = m_player->m_collisionBox;
		float leftX = pos.x + cb.x;
		float rightX = pos.x + cb.x + cb.w;
		float y = pos.y + cb.y;
		
		float newY = y - moveSpeed;
		
		bool walkable = m_worlds[0]->isWalkable(leftX, newY) &
			m_worlds[0]->isWalkable(rightX, newY);
		
		if (walkable)
		{
			posOffset += vec2<float>(0, -moveSpeed);
		}
	}
	// Down.
	if (Input::getSingleton().isKeyHeld(SDLK_s))
	{
		Rect cb = m_player->m_collisionBox;
		float leftX = pos.x + cb.x;
		float rightX = pos.x + cb.x + cb.w;
		float y = pos.y + cb.y + cb.h;
		
		float newY = y + moveSpeed;
		
		bool walkable = m_worlds[0]->isWalkable(leftX, newY) &
			m_worlds[0]->isWalkable(rightX, newY);
		
		if (walkable)
		{
			posOffset += vec2<float>(0, moveSpeed);
		}
	}
	// Right.
	if (Input::getSingleton().isKeyHeld(SDLK_d))
	{
		Rect cb = m_player->m_collisionBox;
		float x = pos.x + cb.x + cb.w;
		float topY = pos.y + cb.y;
		float botY = pos.y + cb.y + cb.h;
		
		float newX = x + moveSpeed;
		
		bool walkable = m_worlds[0]->isWalkable(newX, topY) &
			m_worlds[0]->isWalkable(newX, botY);
		
		if (walkable)
		{
			posOffset += vec2<float>(moveSpeed, 0);
		}
	}
	// Left.
	if (Input::getSingleton().isKeyHeld(SDLK_a))
	{
		Rect cb = m_player->m_collisionBox;
		float x = pos.x + cb.x;
		float topY = pos.y + cb.y;
		float botY = pos.y + cb.y + cb.h;
		
		float newX = x - moveSpeed;
		
		bool walkable = m_worlds[0]->isWalkable(newX, topY) &
			m_worlds[0]->isWalkable(newX, botY);
		
		if (walkable)
		{
			posOffset += vec2<float>(-moveSpeed, 0);
		}
	}
	
	// Cap the movement speed (for diagonal movement).
	if (length2(posOffset) > moveSpeed * moveSpeed)
	{
		posOffset *= (moveSpeed / length(posOffset));
	}
	m_player->setPos(pos + posOffset);
	
	// Polarity toggle.
	if (Input::getSingleton().isKeyDown(SDLK_SPACE))
	{
		m_player->switchPolarity();
	}
	
	// Inventory display toggle.
	if (Input::getSingleton().isKeyDown(SDLK_i))
	{
		GUI::getSingleton().toggleInventory();
	}
	
	// Rotation. (for simplicity, we're just assuming the player is ALWAYS in the center of screen).
	//{
	//	float x1 = m_input->getMouseX() - 400.0f;
	//	float y1 = SCREEN_HEIGHT - m_input->getMouseY() - 300.0f;

	//	float len = sqrt(x1 * x1 + y1 * y1);

	//	m_player->m_direction = 180 * acos(y1 / len) / 3.14159265f;
	//	if (x1 > 0)
	//		m_player->m_direction = 360 - m_player->m_direction;
	//}
	
	// Fire bullets or interact with NPC.
	//	if (m_input->isMouseButtonHeld(SDL_BUTTON_LEFT))
	if (Input::getSingleton().isMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if (!handleTargeting(Input::getSingleton().getMousePos()))
		{
			static const float BULLET_SPEED = 150;

			vec2<float> temp(Input::getSingleton().getMousePos().x - SCREEN_WIDTH / 2.f,
							 SCREEN_HEIGHT - Input::getSingleton().getMousePos().y - SCREEN_HEIGHT / 2.f);
			temp = normalize(temp);
			
			// TODO: eventually, we need to make a single memory pool. We can't have this much dynamic allocation.
			// Maybe we can create a projectile manager or something.
			vec2<float> vel(temp.x * BULLET_SPEED, -temp.y * BULLET_SPEED);
			ProjectileManager::getSingleton().addProjectile(m_player, m_player->getPos(), vel);
		}
	}
	
	// Spell gestures.
	if (Input::getSingleton().isMouseButtonHeld(SDL_BUTTON_RIGHT))
	{
		GestureSystem::Point2D p(Input::getSingleton().getMousePos().to<float>());
		m_currentGesture.push_back(p);
	}
	else if (Input::getSingleton().isMouseButtonUp(SDL_BUTTON_RIGHT))
	{
		// TODO: target is always the player (we need to implement target system).
		if (!SpellSystem::getSingleton().cast(m_player, m_player, m_currentGesture))
			GUI::getSingleton().addToChat("Spell casting failed!");
		
		m_currentGesture.clear();
	}
}

void Application::drawScene()
{
	switch (m_currentMode)
	{
	case INTRO_SCREEN:
	case CREDITS:
	case OPTIONS:
		break;

	case GAME:
		{
			glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
			glLoadIdentity();

			const vec2<float> playerPos = m_player->getPos();

			// Camera.
			glTranslatef(-playerPos.x + SCREEN_WIDTH / 2, -playerPos.y + SCREEN_HEIGHT / 2, 0);

			m_worlds[0]->renderLayer0();
			m_worlds[0]->renderLayer1();

			// Player.
			glPushMatrix();
			glTranslatef(playerPos.x, playerPos.y, 0);
			//	glRotatef(m_player->m_direction, 0, 0, -1);
			//	glTranslatef(-14.5, -25, 0);

			Renderer::getSingleton().blit(m_player->getImage(), 0, 0);
			glPopMatrix();

			// NPCs.
			std::vector< EntityBase* > activeEntities = getCurrentEntities();
			for (Uint16 i = 0; i < activeEntities.size(); ++i)
			{
				const vec2<float> pos = activeEntities[i]->getPos();
				Renderer::getSingleton().blit(activeEntities[i]->getImage(), (int)pos.x, (int)pos.y);
			}

			m_worlds[0]->renderLayer2();

			// Projectiles.
			// TODO: have projectileManager just render all the projectiles itself.
			{
				std::list< Projectile* > projectiles = ProjectileManager::getSingleton().getProjectiles();
				std::list< Projectile* >::iterator iter;
				for (iter = projectiles.begin(); iter != projectiles.end(); ++iter)
				{
					const vec2<float> pos = (*iter)->getPos();
					Renderer::getSingleton().blit((*iter)->getImage(),
						(int)pos.x - 3,
						(int)pos.y
						);
				}
			}

			drawCollisionBox();
			drawTargetBox();
			drawSpellGesture();

			glPopAttrib();
			glPopMatrix();
		}
		break;
	}
}

void Application::drawCollisionBox()
{
	//glPushMatrix();

	//glDisable(GL_TEXTURE_2D);

	//Rect cb = m_player->m_collisionBox;
	//int rightX = (int)m_player->m_posX + cb.x + cb.w;
	//int topY = (int)m_player->m_posY + cb.y;
	//int botY = (int)m_player->m_posY + cb.y + cb.h;
	//int leftX = (int)m_player->m_posX + cb.x;

	//glColor3f(1, 0, 0);
	//glBegin( GL_LINE_LOOP );
	//	glVertex2i(leftX, topY);
	//	glVertex2i(rightX, topY);
	//	glVertex2i(rightX, botY);
	//	glVertex2i(leftX, botY);
	//glEnd();

	//glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
}

void Application::drawTargetBox()
{
	//glPushMatrix();

	//glDisable(GL_TEXTURE_2D);

	//std::vector< EntityBase* > activeEntities = m_worlds[0]->getCurrentEntities();
	//for (Uint16 i = 0; i < activeEntities.size(); ++i)
	//{
	//	EntityBase *e = activeEntities[i];
	//	Rect cb = e->m_targetBox;
	//	int rightX = (int)e->m_posX + cb.x + cb.w;
	//	int topY = (int)e->m_posY + cb.y;
	//	int botY = (int)e->m_posY + cb.y + cb.h;
	//	int leftX = (int)e->m_posX + cb.x;

	//	glColor3f(0, 0, 1);
	//	glBegin( GL_LINE_LOOP );
	//	glVertex2i(leftX, topY);
	//	glVertex2i(rightX, topY);
	//	glVertex2i(rightX, botY);
	//	glVertex2i(leftX, botY);
	//	glEnd();
	//}

	//glEnable(GL_TEXTURE_2D);

	//glPopMatrix();
}

std::vector< EntityBase* > Application::getCurrentEntities()
{
	// TODO: eventually we should prune (i.e. Only return entities near the player?).
	std::vector< EntityBase* > ents = m_worlds[0]->getCurrentEntities();

	ents.push_back(m_player);

	return ents;
}

bool Application::handleTargeting(const vec2<int> &mousePos)
{
	// Targeting system.
	// TODO: - in the future, we may have issues with overlapping objects. We will
	//			want to select the object "closer to the camera". I think we can
	//			just look at the y coordinate, and take the object with the largest y
	//			(lower on the screen, which means closer to the camera using the perspective).

	// Translate mouse coords to world-coordinates.
	vec2<float> playerPos = m_player->getPos();
	vec2<int> wc(mousePos.x - (int)(-playerPos.x + SCREEN_WIDTH / 2),
				mousePos.y - (int)(-playerPos.y + SCREEN_HEIGHT / 2));

	std::vector< EntityBase* > activeEntities = getCurrentEntities();
	for (uint i = 0; i < activeEntities.size(); ++i)
	{
		Rect tb = activeEntities[i]->m_targetBox;

		// Translate targetbox to world-coordinates.
		tb.x += (int)activeEntities[i]->getPos().x;
		tb.y += (int)activeEntities[i]->getPos().y;

		// Click on something?
		if (wc.x >= (int)tb.x && wc.x <= (int)(tb.x + tb.w) &&
			wc.y >= (int)tb.y && wc.y <= (int)(tb.y + tb.h))
		{
			if (dynamic_cast<NPC*>(activeEntities[i]) && !activeEntities[i]->isCombative())
			{
				std::string txt = activeEntities[i]->getName() + " selected!";
				GUI::getSingleton().addToChat(txt);
				return true;
			}
			// Try to loot dead NPCs.
			else if (dynamic_cast<NPC*>(activeEntities[i]) && !activeEntities[i]->isAlive())
			{
				// TODO:
				// if within range, (open GUI w/ inventory of killed NPC), (gui will handled all clicks)
				float dist2 = length2(activeEntities[i]->getPos() - m_player->getPos());
				const float LOOT_RANGE = 100;
				if (dist2 <= LOOT_RANGE * LOOT_RANGE)
				{
					printf("Loot!\n");
					activeEntities[i]->m_inventory->printInventory();
					return true;
				}
				else
					printf("Too far!\n");
			}

			// Selected a hostile NPC (don't select it!).
			return false;
		}
	}
	return false;
}

void Application::drawSpellGesture()
{
	// TODO: color should be dependent on your current POLARITY.

	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin( GL_LINE_STRIP );
	for (uint i = 0; i < m_currentGesture.size(); ++i)
	{
		glVertex2i((int)m_currentGesture[i].x, (int)m_currentGesture[i].y);
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glPopMatrix();
}
#include "World.h"

#include "Renderer.h"

#include "tinyxml/tinyxml.h"

World::World(Renderer *renderer, Player *player)
{
	assert(renderer);
	assert(player);

	m_renderer = renderer;
	m_player = player;
}

World::~World()
{
	// Clean up.
	for (Uint16 i = 0; i < m_tiles.size(); ++i)
	{
		for (Uint16 j = 0; j < m_tiles[i].size(); ++j)
		{
			delete m_tiles[i][j];
		}
	}
}

bool World::load(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument(fileName);
	if (!doc->LoadFile())
		return false;	// TODO: change to exception in all load funcs

	TiXmlElement *elem = doc->FirstChildElement();

	while (elem)
	{
		if (strcmp(elem->Value(), "npcs") == 0)
		{
			std::string file = elem->Attribute("file");
			loadNPCs(file);
		}
		else if (strcmp(elem->Value(), "objects") == 0)
		{
			int blah = 0;
		}
		else if (strcmp(elem->Value(), "loot") == 0)
		{
			int blah = 0;
		}
		else if (strcmp(elem->Value(), "actions") == 0)
		{
			int blah = 0;
		}
		else if (strcmp(elem->Value(), "world") == 0)
		{
			std::string file = elem->Attribute("file");
			loadWorld(file);
		}

		elem = dynamic_cast<TiXmlElement*>(elem->NextSibling());
	}

	return true;
}

void World::loadNPCs(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument("data/" + fileName);
	doc->LoadFile();

	TiXmlElement *elem = doc->FirstChildElement();

	while (elem)
	{
		assert(strcmp(elem->Value(), "npc") == 0);

		// TODO: creating a temporary NPC enemy for testing
		/*	NPC *n1 = new NPC();*/
		//	n1->m_image = m_renderer->loadImage("enemy.png");
		//	n1->m_tileX = 1; n1->m_tileY = 1;
		//	n1->m_loc.x = 483;	n1->m_loc.y = 396;
		//	n1->m_loc.w = n1->m_image->w;
		//	n1->m_loc.h = n1->m_image->h;
		//	n1->m_currentState = EntityBase::WORLD;
		//	m_tiles[1][1]->npcs.push_back(n1);

		NPC *npc = new NPC(elem->Attribute("name"));


		elem = dynamic_cast<TiXmlElement*>(elem->NextSibling());
	}
}

void World::loadWorld(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument("data/" + fileName);
	doc->LoadFile();
//	if (!doc->LoadFile())
//		return false;

	std::vector< Tile* > tempTileArray;

	TiXmlElement *root = doc->RootElement();

	Uint16 numRows = 0;
	Uint16 numColsInRow[255];	// We assume numRows will be < 255.
	for (Uint16 i = 0; i < 255; ++i)
		numColsInRow[i] = 0;

	if (strcmp(root->Value(), "world") == 0)
	{
		m_name = root->Attribute("name");

		// Loop through each tile.
		TiXmlElement *tile = root->FirstChildElement();
		while (tile)
		{
			if (strcmp(tile->Value(), "tile") == 0)
			{
				Tile *t = new Tile;
				tile->QueryIntAttribute("x", &t->x);
				tile->QueryIntAttribute("y", &t->y);

				t->worldImage = new Image;
				m_renderer->loadImage(tile->Attribute("texture"), t->worldImage);
				//	if (t->image == NULL)
				//		return false;

				tile->QueryValueAttribute("east", &t->east);
				tile->QueryValueAttribute("west", &t->west);
				tile->QueryValueAttribute("north", &t->north);
				tile->QueryValueAttribute("south", &t->south);

				// TODO:
				t->horizon = .1f;

				std::string combat = tile->Attribute("combat");
				if (combat == "")
					t->combatAllowed = false;
				else
				{
					t->combatAllowed = true;
					//		t->combatImage = m_renderer->loadImage(combat);
//					if (t->combatImage == NULL)
//						return false;
				}

				tempTileArray.push_back(t);

				// Count # of rows used.
				if (t->x + 1 > numRows)
					numRows = t->x + 1;

				// Count # of cols used.
				if (t->y + 1 > numColsInRow[t->x])
					numColsInRow[t->x] = t->y + 1;
			}

			tile = dynamic_cast<TiXmlElement*>(root->IterateChildren(dynamic_cast<TiXmlNode*>(tile)));
		}
	}

	// Allocates the correctly sized arrays.
	for (Uint16 i = 0; i < numRows; ++i)
	{
		std::vector< Tile* > colVec;
		for (Uint16 j = 0; j < numColsInRow[i]; ++j)
		{
			colVec.push_back(NULL);
		}

		m_tiles.push_back(colVec);
	}

	// Fill into the main array.
	for (unsigned i = 0; i < tempTileArray.size(); ++i)
		m_tiles[tempTileArray[i]->x][tempTileArray[i]->y] = tempTileArray[i];
}

void World::renderWorld()
{
	Tile *cur = getCurrentTile();

	m_renderer->draw(cur->worldImage, cur->worldImage->w / 2, cur->worldImage->h);

	m_renderer->draw(m_player->m_image, (int)m_player->m_locX, (int)m_player->m_locY);

//	SDL_Rect re = m_player->getLoc();
//	re.h = 200;
//	m_renderer->draw(m_player->m_image,re /*m_player->getLoc()*/);
//
//	for (Uint16 i = 0; i < getCurrentTile()->npcs.size(); ++i)
//	{
//		NPC *npc = getCurrentTile()->npcs[i];
//		m_renderer->draw(npc->m_image, npc->getLoc());
//	}

	// TODO: also eventually need to draw all objects/NPCs on screen
}

void World::renderCombat()
{
//	m_renderer->draw(getCurrentTile()->combatImage, 0, 0);
//
//	m_renderer->draw(m_player->m_image, m_player->getLoc());
//
//	// TODO: need to draw all active combatants
//	
//	for (Uint16 i = 0; i < getCurrentTile()->npcs.size(); ++i)
//	{
//		NPC *npc = getCurrentTile()->npcs[i];
//		m_renderer->draw(npc->m_image, npc->getLoc());
//	}
}

bool World::isMoveAllowed(DIRECTION dir)
{
	bool res = false;

	switch (dir)
	{
	case NORTH:
		res = getCurrentTile()->north;
		break;

	case SOUTH:
		res = getCurrentTile()->south;
		break;

	case EAST:
		res = getCurrentTile()->east;
		break;

	case WEST:
		res = getCurrentTile()->west;
		break;
	}

	return res;
}

std::vector< EntityBase* > World::getCurrentEntities()
{
	std::vector< EntityBase* > res;

	Tile *t = getCurrentTile();
	
	// All the NPCs...
	for (Uint16 i = 0; i < t->npcs.size(); ++i)
		res.push_back(t->npcs[i]);

	// TODO: All the objects...
	// TODO: All the other players...
	res.push_back(m_player);

	return res;
}
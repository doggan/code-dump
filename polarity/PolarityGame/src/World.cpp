#include "World.h"

#include "ImageManager.h"
#include "NPCManager.h"
#include "Renderer.h"
#include "Singleton.h"

#include "SDL_image.h"

#include "tinyxml.h"

World::World()
{
}

World::World(const std::string &worldName, vec2<uint> tileNum, vec2<uint> tileDim)
{
	m_tiles.resize(tileNum.x);
	for (uint x = 0; x < tileNum.x; ++x)
		m_tiles[x].resize(tileNum.y);

	for (uint x = 0; x < tileNum.x; ++x)
		for (uint y = 0; y < tileNum.y; ++y)
			m_tiles[x][y] = new Tile;

	m_name = worldName;
	m_tileNum = tileNum;
	m_tileDim = tileDim;
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

void World::load(const std::string &fileName)
{
	TiXmlDocument *doc = new TiXmlDocument(fileName);
	if (!doc->LoadFile())
		EXCEPTION(Exception::ERR_FILE_NOT_FOUND, "loadWorld()", "Cannot load file: " + fileName);

	m_fileName = fileName;

	TiXmlElement *root = doc->RootElement();

	if (root->ValueStr() == "world")
	{
		m_name = root->Attribute("name");

		// Allocate tile memory.
		{
			root->QueryValueAttribute("sizeX", &m_tileNum.x);
			root->QueryValueAttribute("sizeY", &m_tileNum.y);

			root->QueryValueAttribute("tileDimX", &m_tileDim.x);
			root->QueryValueAttribute("tileDimY", &m_tileDim.y);

			m_tiles.resize(m_tileNum.x);
			for (uint i = 0; i < m_tileNum.x; ++i)
				m_tiles[i].resize(m_tileNum.y);

			for (uint i = 0; i < m_tileNum.x; ++i)
				for (uint j = 0; j < m_tileNum.y; ++j)
					m_tiles[i][j] = new Tile;
		}

		// Read in the world properties.
		TiXmlElement *child = root->FirstChildElement();
		while (child)
		{
			if (child->ValueStr() == "tile")
			{
				Tile *t = new Tile;
				child->QueryIntAttribute("x", &t->x);
				child->QueryIntAttribute("y", &t->y);

				std::string layer;
				if (child->QueryValueAttribute("base", &layer) == TIXML_SUCCESS)
				{
					ImageManager::getSingleton().getImage(layer, t->bgLayer);
				}

				if (child->QueryValueAttribute("fringe", &layer) == TIXML_SUCCESS)
				{
					ImageManager::getSingleton().getImage(layer, t->fringeLayer);
				}

				if (child->QueryValueAttribute("fore", &layer) == TIXML_SUCCESS)
				{
					ImageManager::getSingleton().getImage(layer, t->foreLayer);
				}

				child->QueryValueAttribute("walkable", &t->walkable);

				m_tiles[t->x][t->y] = t;
			}
			else if (child->ValueStr() == "npc")
			{
				int id;
				child->QueryIntAttribute("id", &id);

				NPC *npc = NPCManager::getSingleton().getObject(id);

				m_npcs.insert(npc);

				std::string behavior = child->Attribute("behavior");
				if (behavior == "roam")
					npc->m_behavior = NPC::ROAM;
				else if (behavior == "stay")
					npc->m_behavior = NPC::STAY;
				else
					EXCEPTION(Exception::ERR_NOT_IMPLEMENTED, "loadWorld()", "Behavior, " + behavior + ", not yet implemented.");

				float speed;
				child->QueryValueAttribute("speed", &speed);
				npc->setSpeed(speed);

				vec2<float> pos;
				child->QueryValueAttribute("x", &pos.x);
				child->QueryValueAttribute("y", &pos.y);
				npc->setPos(pos);
				
				// TODO:
				npc->m_pos1.x = pos.x;
				npc->m_pos1.y = pos.y;

				child->QueryValueAttribute("x2", &pos.x);
				child->QueryValueAttribute("y2", &pos.y);

				// TODO:
				npc->m_pos2.x = pos.x;
				npc->m_pos2.y = pos.y;
			}

			child = dynamic_cast<TiXmlElement*>(root->IterateChildren(dynamic_cast<TiXmlNode*>(child)));
		}
	}

	delete doc;
}

void World::save(const std::string &fileName)
{
	TiXmlDocument doc;

	TiXmlElement *world = new TiXmlElement("world");
	world->SetAttribute("name", m_name);
	world->SetAttribute("sizeX", m_tileNum.x);
	world->SetAttribute("sizeY", m_tileNum.y);
	world->SetAttribute("tileDimX", m_tileDim.x);
	world->SetAttribute("tileDimY", m_tileDim.y);
	doc.LinkEndChild(world);

	// Tiles.
	{
		for (uint x = 0; x < m_tiles.size(); ++x)
		{
			for (uint y = 0; y < m_tiles[x].size(); ++y)
			{
				TiXmlElement *tile = new TiXmlElement("tile");
				tile->SetAttribute("x", x);
				tile->SetAttribute("y", y);
				
				if (m_tiles[x][y]->bgLayer)
					tile->SetAttribute("base", m_tiles[x][y]->bgLayer->name);

				if (m_tiles[x][y]->fringeLayer)
					tile->SetAttribute("fringe", m_tiles[x][y]->fringeLayer->name);

				if (m_tiles[x][y]->foreLayer)
					tile->SetAttribute("fore", m_tiles[x][y]->foreLayer->name);

				tile->SetAttribute("walkable", m_tiles[x][y]->walkable);
				world->LinkEndChild(tile);
			}
		}
	}

	doc.SaveFile(fileName);
}

void World::renderLayer0()
{
	// Render background.
	for (uint i = m_renderWindow.x; i < m_renderWindow.h; ++i)
	{
		for (uint j = m_renderWindow.y; j < m_renderWindow.w; ++j)
		{
			if (m_tiles[i][j]->bgLayer)
				Renderer::getSingleton().blit(m_tiles[i][j]->bgLayer, j * m_tileDim.y, i * m_tileDim.x);
		}
	}
}

void World::renderLayer1()
{
	// Render fringe objects.
	for (uint i = m_renderWindow.x; i < m_renderWindow.h; ++i)
	{
		for (uint j = m_renderWindow.y; j < m_renderWindow.w; ++j)
		{
			if (m_tiles[i][j]->fringeLayer)
				Renderer::getSingleton().blit(m_tiles[i][j]->fringeLayer, j * m_tileDim.y, i * m_tileDim.x);
		}
	}
}

void World::renderLayer2()
{
	// Render foreground objects.
	for (uint i = m_renderWindow.x; i < m_renderWindow.h; ++i)
	{
		for (uint j = m_renderWindow.y; j < m_renderWindow.w; ++j)
		{
			if (m_tiles[i][j]->foreLayer)
				Renderer::getSingleton().blit(m_tiles[i][j]->foreLayer, j * m_tileDim.y, i * m_tileDim.x);
		}
	}
}

void World::renderCollisionLayer(const Image *collImg)
{
	// Render an X on each tile where collision is active.
	for (uint i = m_renderWindow.x; i < m_renderWindow.h; ++i)
	{
		for (uint j = m_renderWindow.y; j < m_renderWindow.w; ++j)
		{
			if (!m_tiles[i][j]->walkable)
				Renderer::getSingleton().blit(collImg, j * m_tileDim.y, i * m_tileDim.x);
		}
	}
}

bool World::isWalkable(float x, float y)
{
	int tileNumX = (int)(x / m_tileDim.x);
	int tileNumY = (int)(y / m_tileDim.x);

	return m_tiles[tileNumY][tileNumX]->walkable;
}

std::vector< EntityBase* > World::getCurrentEntities()
{
	std::vector< EntityBase* > res;

	// All the npcs...
	std::set< NPC* >::const_iterator iter = m_npcs.begin();
	for (iter; iter != m_npcs.end(); ++iter)
		res.push_back(*iter);

	return res;
}

vec2<uint> World::worldToTileCoords(vec2<uint> worldCoords)
{
	vec2<uint> coord = worldCoords / getTileDimensions();

	// We need to switch the coords, since world coordinates are
	// X = left/right, Y = top/down.. and tile coords are X = row (top/down)
	// and Y = column (left/right).
	return vec2<uint>(coord.y, coord.x);
}

std::string World::getTileImage(vec2<uint> tilePos, TILE_LAYER layer)
{
	switch (layer)
	{
	case LAYER_0:
		if (m_tiles[tilePos.x][tilePos.y]->bgLayer)
			return m_tiles[tilePos.x][tilePos.y]->bgLayer->name;
		break;

	case LAYER_1:
		if (m_tiles[tilePos.x][tilePos.y]->fringeLayer)
			return m_tiles[tilePos.x][tilePos.y]->fringeLayer->name;
		break;

	case LAYER_2:
		if (m_tiles[tilePos.x][tilePos.y]->foreLayer)
			return m_tiles[tilePos.x][tilePos.y]->foreLayer->name;
		break;
	}

	return "";
}

void World::setTileImage(vec2<uint> tilePos, TILE_LAYER layer, const std::string &textureName)
{
	Image *img;
	ImageManager::getSingleton().getImage(textureName, img);

	switch (layer)
	{
	case LAYER_0:
		m_tiles[tilePos.x][tilePos.y]->bgLayer = img;
		break;

	case LAYER_1:
		m_tiles[tilePos.x][tilePos.y]->fringeLayer = img;
		break;

	case LAYER_2:
		m_tiles[tilePos.x][tilePos.y]->foreLayer = img;
		break;
	}
}

void World::toggleCollision(vec2<uint> tilePos)
{
	m_tiles[tilePos.x][tilePos.y]->walkable = !m_tiles[tilePos.x][tilePos.y]->walkable;
}

void World::setRenderArea(vec2<float> camera, vec2<uint> window)
{
	// Vertical.
	int top = static_cast<int>(camera.y / m_tileDim.y);
	int bottom = top + static_cast<int>(window.y / m_tileDim.y) + 1;

	// Horizontal.
	int left = static_cast<int>(camera.x / m_tileDim.x);
	int right = left + static_cast<int>(window.x / m_tileDim.x);

	if (top < 0)					top = 0;
	if (bottom > (int)m_tileNum.x)	bottom = m_tileNum.x;
	if (left < 0)					left = 0;
	if (right > (int)m_tileNum.y)	right = m_tileNum.y;

	m_renderWindow.x = top;
	m_renderWindow.h = bottom;

	m_renderWindow.y = left;
	m_renderWindow.w = right;
}
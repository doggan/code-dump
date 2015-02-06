#ifndef _World_h_
#define _World_h_

#include "Common.h"

#include "vector_math.h"

#include <string>
#include <vector>
#include <set>

// Forward declarations.
class EntityBase;
class NPC;

// TODO: probably create a world manager. The world manager should handle loading of the worlds.

class World
{
public:
	World();
	World(const std::string &worldName, vec2<uint> tileNum, vec2<uint> tileDim);
	virtual ~World();

	void load(const std::string &fileName);
	void save(const std::string &fileName);

	void setRenderArea(vec2<float> camera, vec2<uint> window);
	void renderLayer0();	// Background.
	void renderLayer1();	// Fringe.
	void renderLayer2();	// Foreground.
	void renderCollisionLayer(const Image *collImg);

	std::vector< EntityBase* > getCurrentEntities();

	vec2<uint> getNumberOfTiles() { return m_tileNum; }
	vec2<uint> getTileDimensions() { return m_tileDim; }

	vec2<uint> worldToTileCoords(vec2<uint> mouseCoords);

	// Is the given point a walkable region?
	bool isWalkable(float x, float y);

	// Used by the editor.
	std::string getTileImage(vec2<uint> tilePos, TILE_LAYER layer);
	void setTileImage(vec2<uint> tilePos, TILE_LAYER layer, const std::string &textureName);
	void toggleCollision(vec2<uint> tilePos);

	std::string getFileName() { return m_fileName; }

private:
	std::string m_name;
	std::string m_fileName;

	vec2<uint> m_tileNum; // The # of tiles in x/y.
	vec2<uint> m_tileDim; // The size of each tile (in pixels).

	Rect m_renderWindow;

	struct Tile
	{
		Tile() {
			bgLayer = fringeLayer = foreLayer = NULL;
			walkable = true;
		}

		virtual ~Tile() {}

		int x, y;	// This tile's position in the world.
		Image *bgLayer, *fringeLayer, *foreLayer;
		bool walkable;
	};

	std::set< NPC* > m_npcs;

	std::vector< std::vector< Tile* > > m_tiles;
};

#endif // _World_h_
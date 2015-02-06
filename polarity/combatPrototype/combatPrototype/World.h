#ifndef _World_h_
#define _World_h_

#include "Common.h"
#include "EntityBase.h"

#include "SDL/SDL_image.h"

#include <string>
#include <vector>

class Renderer;

class World
{
public:
	World(Renderer *renderer, Player *player);
	virtual ~World();

	// TODO: pepper these load functions with 'throws' for file-not-found errors.
	bool load(const std::string &fileName);

	void renderWorld();
	void renderCombat();

	enum DIRECTION
	{
		NORTH,
		SOUTH,
		EAST,
		WEST
	};

	bool isMoveAllowed(DIRECTION dir);
	bool isCombatAllowed() { return getCurrentTile()->combatAllowed; }
	float getHorizon() { return getCurrentTile()->horizon; }
	std::vector< EntityBase* > getCurrentEntities();

private:
	void loadWorld(const std::string &fileName);
	void loadNPCs(const std::string &fileName);

	std::string m_name;

	struct Tile
	{
		int x, y;	// This tile's position in the world.
		Image *worldImage;
		SDL_Surface *combatImage;
		bool east, west, north, south;
		bool combatAllowed;
		float horizon;
		std::vector< NPC* > npcs;
		// TODO: later object array will go here as well.
	};

	Tile *getCurrentTile() { return m_tiles[m_player->m_tileX][m_player->m_tileY]; }

	std::vector< std::vector< Tile* > > m_tiles;

	Renderer *m_renderer;
	Player *m_player;
};

#endif // _World_h_
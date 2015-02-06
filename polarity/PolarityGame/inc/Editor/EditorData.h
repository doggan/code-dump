#ifdef EDITOR

#ifndef _EditorData_h_
#define _EditorData_h_

#include "Common.h"
#include "Singleton.h"

#include "wx/bitmap.h"

#include <vector>

class EditorData : public Singleton< EditorData >
{
public:
	struct Tile
	{
		Tile() { img = NULL; }
		~Tile() { if (img) delete img; }

		std::string name;
		wxBitmap *img;
		Rect loc;
	};

private:
	bool m_worldLoaded;
	bool m_showCollision;
	bool m_showLayer[3];
	bool m_drawGrid;
	Tile *m_activeTile;
	TILE_LAYER m_activeLayer;
	std::vector< EditorData::Tile* > m_loadedTiles;

public:
	void setWorldLoaded(bool state) { m_worldLoaded = state; }
	bool isWorldLoaded() { return m_worldLoaded; }

	void setShowCollision(bool state) { m_showCollision = state; }
	bool isShowCollision() { return m_showCollision; }

	void setShowLayer(bool state, uint layer) { m_showLayer[layer] = state; }
	bool isShowLayer(uint layer) { return m_showLayer[layer]; }

	void setDrawGrid(bool state) { m_drawGrid = state; }
	bool isDrawGrid() { return m_drawGrid; }

	void setActiveTile(Tile *activeTile) { m_activeTile = activeTile; }
	Tile *getActiveTile() { return m_activeTile; }

	void setActiveLayer(TILE_LAYER layer) { m_activeLayer = layer; }
	TILE_LAYER getActiveLayer() { return m_activeLayer; }

	void addTile(Tile *tile) { m_loadedTiles.push_back(tile); }
	std::vector< Tile* > getTile() { return m_loadedTiles; }
	Tile *getTile(const std::string &name);
	Tile *getTile(uint x, uint y);

	EditorData()
	{
		m_worldLoaded = false;
		m_showCollision = true;
		m_showLayer[0] = m_showLayer[1] = m_showLayer[2] = true;
		m_drawGrid = true;
		m_activeTile = NULL;
		m_activeLayer = LAYER_0;
	}

	~EditorData()
	{
		for (uint i = 0; i < m_loadedTiles.size(); ++i)
		{
			delete m_loadedTiles[i];
		}
	}
};

#endif // _EditorData_h_

#endif
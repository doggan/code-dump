#ifdef EDITOR

#include "EditorData.h"

EditorData::Tile *EditorData::getTile(const std::string &name)
{
	for (uint i = 0; i < m_loadedTiles.size(); ++i)
	{
		if (m_loadedTiles[i]->name == name)
		{
			return m_loadedTiles[i];
		}
	}

	return NULL;
}

EditorData::Tile *EditorData::getTile(uint x, uint y)
{
	for (uint i = 0; i < m_loadedTiles.size(); ++i)
	{
		Rect r = m_loadedTiles[i]->loc;

		if (x >= r.x && x < r.x + r.w &&
			y >= r.y && y < r.y + r.h)
		{
			return m_loadedTiles[i];
		}
	}

	return NULL;
}

#endif
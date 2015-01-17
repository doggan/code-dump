#include "a_star_search.h"

const int MAP_HEIGHT = 5;
const int MAP_WIDTH = 5;
int gTestMap[MAP_WIDTH * MAP_HEIGHT] =
{
//  0  1  2  3  4
    1, 0, 1, 1, 1,  // 0
    1, 0, 1, 1, 1,  // 1
    1, 1, 1, 1, 1,  // 2
    1, 1, 1, 1, 1,  // 3
    1, 1, 1, 1, 1,  // 4
};

class TileSearchNode
{
public:
    TileSearchNode()
        :mX(0)
        ,mY(0)
    {}

    TileSearchNode(int32_t x, int32_t y)
        :mX(x)
        ,mY(y)
    {}

    bool isSame(const TileSearchNode& node)
    {
        return mX == node.mX &&
               mY == node.mY;
    }

    float getCost(const TileSearchNode& toNode)
    {
        return 1.0f;
    }

    float getCostEstimateToGoal(const TileSearchNode& goalNode)
    {
        // TODO:
        return 0.0f;
    }

    static bool isValidTile(int32_t x, int32_t y)
    {
        int32_t idx = x + y * MAP_WIDTH;
        return gTestMap[idx] != 0;
    }

    void getNeighbors(AStarSearch<TileSearchNode>& aStarSearch)
    {
        // Left.
        if (mX > 0)
        {
            if (isValidTile(mX - 1, mY))
            {
                TileSearchNode n(mX - 1, mY);
                aStarSearch.addNeighbor(n);
            }
        }

        // Right.
        if (mX < MAP_WIDTH - 1)
        {
            if (isValidTile(mX + 1, mY))
            {
                TileSearchNode n(mX + 1, mY);
                aStarSearch.addNeighbor(n);
            }
        }

        // Up.
        if (mY > 0)
        {
            if (isValidTile(mX, mY - 1))
            {
                TileSearchNode n(mX, mY - 1);
                aStarSearch.addNeighbor(n);
            }
        }

        // Down.
        if (mY < MAP_HEIGHT - 1)
        {
            if (isValidTile(mX, mY + 1))
            {
                TileSearchNode n(mX, mY + 1);
                aStarSearch.addNeighbor(n);
            }
        }
    }

    void dump()
    {
     //   std::cout << " x=" << mX << ", y=" << mY << std::endl;
    }

private:
    int32_t mX;
    int32_t mY;
};

class WorldTileNode
{
public:
    WorldTileNode()
        :mX(0)
        ,mY(0)
        ,mLevel(NULL)
    {}

    WorldTileNode(int32_t x, int32_t y, Level::Level* level)
        :mX(x)
        ,mY(y)
        ,mLevel(level)
    {}

    bool isSame(const WorldTileNode& node)
    {
        return mX == node.mX &&
               mY == node.mY;
    }

    float getCost(const WorldTileNode& toNode)
    {
        return 1.0f;
    }

    float getCostEstimateToGoal(const WorldTileNode& goalNode)
    {
        // Manhattan distance.
        return abs(goalNode.mX - mX) + abs(goalNode.mY - mY);
    }

    static bool isValidTile(int32_t x, int32_t y, Level::Level* level)
    {
        return (*level)[x][y].passable();
    }

    void getNeighbors(AStarSearch<WorldTileNode>& aStarSearch)
    {
        // Left.
  //      if (mX > 0)
        {
            if (isValidTile(mX - 1, mY, mLevel))
            {
                WorldTileNode n(mX - 1, mY, mLevel);
                aStarSearch.addNeighbor(n);
            }
        }

        // Right.
//        if (mX < MAP_WIDTH - 1)
        {
            if (isValidTile(mX + 1, mY, mLevel))
            {
                WorldTileNode n(mX + 1, mY, mLevel);
                aStarSearch.addNeighbor(n);
            }
        }

        // Up.
 //       if (mY > 0)
        {
            if (isValidTile(mX, mY - 1, mLevel))
            {
                WorldTileNode n(mX, mY - 1, mLevel);
                aStarSearch.addNeighbor(n);
            }
        }

        // Down.
   //     if (mY < MAP_HEIGHT - 1)
        {
            if (isValidTile(mX, mY + 1, mLevel))
            {
                WorldTileNode n(mX, mY + 1, mLevel);
                aStarSearch.addNeighbor(n);
            }
        }
    }

    void dump()
    {
     //   std::cout << " x=" << mX << ", y=" << mY << std::endl;
    }

public:
    int32_t mX;
    int32_t mY;

    Level::Level* mLevel;
};

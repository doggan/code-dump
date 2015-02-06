#ifndef _EntityBase_h_
#define _EntityBase_h_

#include "Common.h"

#include "SDL_image.h"

#include "vector_math.h"

#include <vector>
#include <string>
#include <queue>

// Forward declarations.
class Inventory;
class Item;

class EntityBase
{
public:
	EntityBase(uint id, const std::string &name = "");
	EntityBase(const EntityBase &eb);
	EntityBase &operator=(const EntityBase &eb);
	virtual ~EntityBase();

	std::string getName() { return m_name; }
	uint getID() { return m_id; }

	virtual void update(float deltaTime);

	void applyStats(const Stats &stats);
	void deapplyStats(const Stats &stats);

	void addToInventory(Item *i, bool autoEquip = false);

	void changeHP(EntityBase *affector, int delta) { m_recentAffector = affector; m_stats.hp += delta; }

	virtual void changeEnergy(int delta) { m_stats.energy += delta; }
	virtual int getEnergy() { return m_stats.energy; }

	bool isCombative() { return m_combative; }
	void setCombative(bool val) { m_combative = val; }

	bool isAlive() { return m_alive; }

	vec2<float> getPos() { return m_pos; }
	void setPos(const vec2<float> &pos) { m_pos = pos; }
	void setPos(const float &x, const float &y) { m_pos.x = x; m_pos.y = y; }

	float getSpeed() { return m_movementSpeed; }
	void setSpeed(float speed) { m_movementSpeed = speed; }

	void setStats(const Stats &stats) { m_stats = stats; }

	uint getGold() { return m_gold; }
	void setGold(uint amount) { m_gold = amount; }

	uint getExp() { return m_xp; }
	void setExp(uint exp) { m_xp = exp; }

	POLARITY getPolarity() const { return m_polarity; }
	void setPolarity(POLARITY p) { m_polarity = p; }

protected:
	std::string m_name;
	uint m_id;

	float m_movementSpeed;

	bool m_combative;

	uint m_xp;	// The player's total experience. OR, for NPCs, the amount of xp gained by Player when the NPC is killed.
	uint m_gold;

	// TODO:
public:
	Inventory *m_inventory;

protected:
	EntityBase *m_recentAffector;

	virtual void kill();

public:
	Rect m_collisionBox;	// Used for environmental collisions.
	Rect m_targetBox;	// Used for projectile collisions.

	POLARITY m_polarity;

	// TODO:
public:
//protected:

	const Image *getImage()
	{
		if (m_polarity == DARK)
			return m_imageDark;
		else
			return m_imageLight;
	}

	Image *m_imageDark, *m_imageLight;

protected:
	vec2<float> m_pos;

	bool m_alive;

public:
	float m_direction;

	EntityBase *m_currentTarget; // The target the this entity is currently selecting.

	Stats m_stats;

public:
	// TODO:
	// I chose to give this to EntityBase, since this might allow objects (doors, boxes)
	//	to have spell casting abilities. This could be used for traps?
	std::vector< uint > m_spellList;	// Unique IDs of spells this Entity has to use.
};

#endif // _EntityBase_h_
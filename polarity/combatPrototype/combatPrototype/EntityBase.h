#ifndef _EntityBase_h_
#define _EntityBase_h_

#include "Common.h"
#include "SpellSystem.h"

#include "SDL/SDL_image.h"

#include <vector>
#include <string>
#include <queue>

class Spell;

class EntityBase
{
public:
	EntityBase(const std::string &name);
	virtual ~EntityBase();

	virtual void cast(Uint16 spellID) {}

	void applySpell(Spell *s);

	void changeHP(int delta) { m_stats.hp += delta; }
	void changeMP(int delta) { m_stats.mp += delta; }

	void setCombatantList(std::vector< EntityBase* > combatantList)
	{
		m_combatants = combatantList;
	}

protected:
	std::vector< EntityBase* > m_combatants;	// Used in COMBAT mode.

	bool m_attackSelf;	// If the NPC has some 'confusion' spell cast on them,
						// this can be used so they also attack their NPC comrades.

	// TODO:
public:
//protected:
	enum STATE
	{
		WORLD,
		COMBAT,
		DEAD
	};

	STATE m_currentState;

	Image *m_image; // The texture used for this Entity.
	Uint16 m_tileX, m_tileY; // Location within the world.
	float m_locX, m_locY; // Location within the current tile.
	float m_newLocX, m_newLocY; // Destination location within current tile (i.e. entity is currently moving).

	std::string m_name;

	EntityBase *m_currentTarget; // The target the this entity is currently selecting.

	struct Stats
	{
		int hp, mp;
		int str, intel;
	};

	Stats m_stats;

	// TODO: - this can be called once per-frame for all active entities.
	//		 - it can take care of updating all 'active spells' on the given entity
	//			- Direct Damage spells: apply damage once, remove from queue
	//			- DoTs: apply damage every second, move to end of queue until time expires
	//			- Buffs: maintain effect, keep moving to end of queue till time expires
	//			- Heal: apply heal effect, remove from queue.
	//		- each spell type could have an update(EntityBase *target) method. It applies
	//			the effect of the spell to the target.
	virtual void update();

	std::queue< Spell* > m_activeSpells;

	// TODO:
	// I chose to give this to EntityBase, since this might allow objects (doors, boxes)
	//	to have spell casting abilities. This could be used for traps?
	std::vector< Uint16 > m_spellList;	// Unique IDs of spells this Entity has to use.
};

// TODO: eventually move Player/Enemy to their own files.
class Player : public EntityBase
{
public:
	Player(const std::string &name = "A Player");
	virtual ~Player() {}

	void cast(Uint16 spellID);
	void update();

	// TODO:
// private:
};

class NPC : public EntityBase
{
public:
	NPC(const std::string &name = "A NPC");
	virtual ~NPC() {}

	void cast(Uint16 spellID);
	void update();
	
private:
	float m_combatTimer;
};

#endif // _EntityBase_h_
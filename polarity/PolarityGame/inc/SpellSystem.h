#ifndef _SpellSystem_h_
#define _SpellSystem_h_

#include "Singleton.h"
#include "EntityBase.h"
#include "GestureSystem.h"

#include <string>
#include <map>
#include <vector>
#include <list>

class EntityBase;

class Spell
{
public:
	// TODO: i think this is unnecessary?
	enum SPELL_TYPE
	{
		PROJECTILE,
		EFFECT
	};

	Spell(const std::string &name, SPELL_TYPE type, uint rank, uint energy);
	virtual ~Spell() {}

	// TODO: applies the effect to the target. If the full effect has been applied, let's return
	//		 NULL. If the full effect has not been applied, return the modified spell to be applied
	//		 next frame.
	//	Returns true if this spell is still active, false if it should end.
	virtual bool update(float deltaTime) = 0;
	
	const std::string &getName() { return m_name; }
	int getEnergyConsumption() { return m_energyConsumption; }

	EntityBase *m_target;
	EntityBase *m_owner;

protected:
	std::string m_name;
	SPELL_TYPE m_spellType;
	uint m_rank;
	int m_energyConsumption;

	bool m_dead;
};

class Spell_DirectDamage : public Spell
{
public:
	Spell_DirectDamage(const std::string &name, SPELL_TYPE type, uint rank, uint energy, uint dmgAmount)
		: Spell(name, type, rank, energy), m_dmgAmount(dmgAmount) {}

	virtual bool update(float deltaTime);

protected:
	uint m_dmgAmount;
};

class Spell_Heal : public Spell
{
public:
	Spell_Heal(const std::string &name, SPELL_TYPE type, uint rank, uint energy, uint healAmount)
		: Spell(name, type, rank, energy), m_healAmount(healAmount) {}

	virtual bool update(float deltaTime);

protected:
	uint m_healAmount;
};

class SpellSystem : public Singleton<SpellSystem>
{
public:
	SpellSystem();
	~SpellSystem();

	void update(float deltaTime);

	bool cast(EntityBase *owner, EntityBase *target, const std::vector< GestureSystem::Point2D > &points);
//	bool cast(NPC *caster, uint spellID);
private:
	GestureSystem *m_gestureSystem;

	/* The Master Spell list. These spells are usable by all NPCs and items in the game.
	*/
	std::vector< Spell* > m_spellList;
	
	/* A subset of the Master Spell list. These spells can be used by the player through the gesture
	* casting system. The gesture index is saved along with the spell.
	*/
	std::map< uint, Spell* > m_playerSpellList;

	/* Current active spells within the game. They will be updated per-frame.
	*/
	std::list< Spell* > m_activeSpells;
};

#endif // _SpellSystem_h_
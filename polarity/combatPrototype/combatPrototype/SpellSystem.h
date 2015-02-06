#ifndef _SpellSystem_h_
#define _SpellSystem_h_

#include "Singleton.h"
#include "EntityBase.h"

#include "SDL/SDL.h"

#include <string>
#include <vector>

class EntityBase;

class Spell
{
public:
	enum SPELL_TYPE
	{
		DIRECT_DAMAGE,
		DOT_DAMAGE,
		BUFF,
		HEAL
		// Steal life (DD),
		// Steal life over time (DoT),
		// Heal over Time (HoT)
	};

	Spell(const std::string &name, Uint16 id, SPELL_TYPE type, Uint8 rank, Uint32 maxEff, Uint16 diff, Uint16 mana);
	virtual ~Spell() {}

	// TODO: applies the effect to the target. If the full effect has been applied, let's return
	//		 NULL. If the full effect has not been applied, return the modified spell to be applied
	//		 next frame.
	Spell *update(EntityBase *target);

	// TODO:
public:
//protected:
	std::string m_name;
	Uint16 m_uniqueID;
	SPELL_TYPE m_spellType;
	Uint8 m_rank;
	Uint32 m_maxEffectiveness;
	Uint16 m_difficulty;
	int m_manaConsumption;

	int m_damage;	// Or heal for a heal spell.
};

// TODO: possibly make a base class, and do similar stuff for weapon system? Armor system? hmm
class SpellSystem : public Singleton<SpellSystem>
{
public:
	SpellSystem();
	virtual ~SpellSystem();

	Spell *getSpell(Uint16 id);

	void apply(Spell *s, EntityBase *source, EntityBase *target, float accuracy);

private:
	std::vector< Spell* > m_spellList;
};

#endif // _SpellSystem_h_
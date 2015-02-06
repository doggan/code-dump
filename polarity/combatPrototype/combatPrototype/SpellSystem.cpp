#include "SpellSystem.h"
#include "Utilities.h"

#include <cassert>

SpellSystem::SpellSystem()
{
	// Create some spells for testing.
	m_spellList.push_back(new Spell("Fire Ball 1", 0, Spell::DIRECT_DAMAGE, 1, 10, 0, 1));
	m_spellList.push_back(new Spell("Fire Ball 2", 1, Spell::DIRECT_DAMAGE, 2, 30, 1, 5));
	m_spellList.push_back(new Spell("Fire Ball 3", 2, Spell::DIRECT_DAMAGE, 3, 80, 2, 10));
	m_spellList.push_back(new Spell("Fire Ball 4", 3, Spell::DIRECT_DAMAGE, 4, 200, 3, 50));
	m_spellList.push_back(new Spell("Fire Ball 5", 4, Spell::DIRECT_DAMAGE, 5, 500, 4, 100));

	m_spellList.push_back(new Spell("Heal 1", 5, Spell::HEAL, 1, 10, 0, 1));
	m_spellList.push_back(new Spell("Heal 2", 6, Spell::HEAL, 2, 30, 1, 5));
	m_spellList.push_back(new Spell("Heal 3", 7, Spell::HEAL, 3, 80, 2, 10));
	m_spellList.push_back(new Spell("Heal 4", 8, Spell::HEAL, 4, 200, 3, 50));
}

SpellSystem::~SpellSystem()
{
	for (Uint16 i = 0; i < m_spellList.size(); ++i)
		delete m_spellList[i];
}

Spell *SpellSystem::getSpell(Uint16 id)
{
	for (Uint16 i = 0; i < m_spellList.size(); ++i)
	{
		if (m_spellList[i]->m_uniqueID == id)
			return m_spellList[i];
	}

	return NULL;
}

void SpellSystem::apply(Spell *s, EntityBase *source, EntityBase *target, float accuracy)
{
	// TODO:
	// DD: determine the damage to be applied. Apply it to target's queue. Take mana from source.
	// DoT: determine dmg to be applied. Apply it to target's queue. Take mana from source.
	// Buff: determine enhancement to be applied. Apply it to target's queue. Take mana from source.
	// Heal: determine heal to be applied. Apply it to target's queue. Take mana from source.

	// We have to create a new instance of this spell so that we can adjust it's parameters
	// without affecting other instances.
	Spell *newSpell = new Spell(*s);

	newSpell->m_damage = Uint32(newSpell->m_maxEffectiveness * accuracy);

	int manaCost = int(-newSpell->m_manaConsumption * (1.0f + (1.0f - accuracy)));
	std::string str1 = intToString(manaCost) + "MP from " + source->m_name;
	printf("%s\n", str1.c_str());

	source->changeMP(manaCost);
	target->applySpell(newSpell);
}

Spell::Spell(const std::string &name, Uint16 id, SPELL_TYPE type, Uint8 rank, Uint32 maxEff, Uint16 diff, Uint16 mana)
: m_name(name),
  m_uniqueID(id),
  m_spellType(type),
  m_rank(rank),
  m_maxEffectiveness(maxEff),
  m_difficulty(diff),
  m_manaConsumption(mana)
{

}

Spell *Spell::update(EntityBase *target)
{
	switch (m_spellType)
	{
	case DIRECT_DAMAGE:
		{
			std::string str1 = "-" + intToString(m_damage) + "HP to " + target->m_name;
			printf("%s\n", str1.c_str());

			target->changeHP(-m_damage);
			return NULL;
		}

	case HEAL:
		{
			std::string str1 = "+" + intToString(m_damage) + "HP to " + target->m_name;
			printf("%s\n", str1.c_str());

			target->changeHP(m_damage);
			return NULL;
		}

	}

	assert(!"Not yet implemented!");
}
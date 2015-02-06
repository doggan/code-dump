#include "SpellSystem.h"

#include "GUI.h"
#include "ProjectileManager.h"

SpellSystem::SpellSystem()
{
	m_gestureSystem = new GestureSystem;

	// Create some spells for testing.
	// TODO: move to XML parsing
	Spell *s = new Spell_DirectDamage("Direct Damage (rank 1)", Spell::PROJECTILE, 1, 20, 15);
	m_spellList.push_back(s);
	m_playerSpellList.insert(std::make_pair(0, s));

	s = new Spell_Heal("Heal (rank 1)", Spell::EFFECT, 1, 10, 10);
	m_spellList.push_back(s);
	m_playerSpellList.insert(std::make_pair(0, s));
}

SpellSystem::~SpellSystem()
{
	// TODO: cleanup
//	for (Uint16 i = 0; i < m_spellList.size(); ++i)
//		delete m_spellList[i];
}

bool SpellSystem::cast(EntityBase *owner, EntityBase *target, const std::vector< GestureSystem::Point2D > &points)
{
	float value;
	uint index = m_gestureSystem->getBestMatch(points, &value);

	if (value < .8f)
		return false;

	// TODO: we'll need to use the factory pattern here, in order to get a new copy of
	//			this type of spell.
	Spell *s = NULL;
	
	if (dynamic_cast<Spell_DirectDamage*>(m_spellList[index]))
		s = new Spell_DirectDamage("Direct Damage (rank 1)", Spell::PROJECTILE, 1, 2, 15);
	else
		s = new Spell_Heal("Heal (rank 1)", Spell::EFFECT, 1, 1, 10);

	if (!s)
	{
		EXCEPTION(Exception::ERR_NOT_IMPLEMENTED, "cast()", "No spell w/ that gesture index found!");
	}

	// Does the entity have resources to cast that spell?
	if (s->getEnergyConsumption() <= owner->getEnergy())
	{
		std::string txt = "Cast: " + s->getName();
		GUI::getSingleton().addToChat(txt);

		m_activeSpells.push_back(s);
		owner->changeEnergy(-s->getEnergyConsumption());
		s->m_target = target;
		s->m_owner = owner;

		return true;
	}
	else
	{
		std::string txt = "Insufficient Energy!";
		GUI::getSingleton().addToChat(txt);

		return false;
	}
}

void SpellSystem::update(float deltaTime)
{
	// Update all active spells.
	std::list< Spell* >::iterator iter = m_activeSpells.begin();

	while (iter != m_activeSpells.end())
	{
		// Dead.
		if (!(*iter)->update(deltaTime))
		{
			delete (*iter);
			iter = m_activeSpells.erase(iter);
		}
		// Still alive; move to next spell.
		else
		{
			++iter;
		}
	}
}

Spell::Spell(const std::string &name, SPELL_TYPE type, uint rank, uint energy)
: m_name(name),
  m_spellType(type),
  m_rank(rank),
  m_energyConsumption(energy)
{
	m_target = NULL;
	m_owner = NULL;
}

bool Spell_DirectDamage::update(float deltaTime)
{
	vec2<float> vel(5, 0);
	ProjectileManager::getSingleton().addProjectile(m_owner, m_owner->getPos(), vel);

	return false;
}

bool Spell_Heal::update(float deltaTime)
{
	m_target->changeHP(m_owner, m_healAmount);
	
	return false;
}
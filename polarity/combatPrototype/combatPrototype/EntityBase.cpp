#include "EntityBase.h"

#include "Application.h"
#include "TypingSystem.h"
#include "GlobalTimer.h"

#include <cmath>

EntityBase::EntityBase(const std::string &name)
: m_name(name)
{
	m_currentState = WORLD;
	m_currentTarget = NULL;

	m_image = new Image;
}

EntityBase::~EntityBase()
{
	if (m_image)
		delete m_image;
}

void EntityBase::applySpell(Spell *s)
{
	m_activeSpells.push(s);
}

void EntityBase::update()
{
	// Apply all active spells to this entity.
	for (Uint8 i = 0; i < m_activeSpells.size(); ++i)
	{
		Spell *s = m_activeSpells.front()->update(this);

		if (s != NULL)
			m_activeSpells.push(s);
		else
			delete m_activeSpells.front();

		m_activeSpells.pop();
	}

	// TODO:
	// - When an NPC dies, we need to give EXP to all player's currently alive and in the same combat area
	// - When an NPC dies, we should be able to say "NPC killed by player_name".
	//		- this means we need to know which player dealt the final blow or spell (could be a DoT)
	// - When a Player dies, we should be able to say "Player killed by npc_name".
	//		- this means we need to know which NPC dealt the final blow or spell (could be a DoT)

	switch (m_currentState)
	{
	case WORLD:
		// Entity can die in WORLD from traps, or poison effects from combat.
		if (m_stats.hp <= 0)
		{
			printf("%s - Dead!\n", m_name.c_str());
			m_currentState = DEAD;
		}
		break;

	case COMBAT:
		if (m_stats.hp <= 0)
		{
			printf("%s - Dead!\n", m_name.c_str());
			m_currentState = DEAD;
		}
		break;

	case DEAD:
		break;
	}

	// Apply any walking movement.
	{
		float velocity = 300.0f * GlobalTimer::getSingleton().getTimeElapsed();
		float dirX = m_newLocX - m_locX;
		float dirY = m_newLocY - m_locY;
		float len = sqrt(dirX * dirX + dirY * dirY);
		if (len > 0.0f)
		{
			dirX /= len;
			dirY /= len;

			m_locX += dirX * velocity;
			m_locY += dirY * velocity;
		}
	}

	// TODO: we'll have to play with this. We probably don't actually want the scale to reach 0.
	// Apply the scale due to the horizon.
	{
		float horizon = Application::getSingleton().getCurrentWorld()->getHorizon();

		// This allows us to have tiles without a horizon line (in a city, etc).
		if (horizon < 0.0f)
			m_image->scale = 1.0f;
		else
		{
			if (m_locY < horizon * SCREEN_HEIGHT)
				m_image->scale = 0.0f;
			else
			{
				float offset = horizon * SCREEN_HEIGHT;
				float t = (m_locY - offset) / (SCREEN_HEIGHT - offset);
				m_image->scale = t;
			}
		}
	}
}

Player::Player(const std::string &name /* = "A Player" */)
: EntityBase(name)
{
	m_stats.hp = 100;
	m_stats.mp = 100;

	// Give the player some spells.
//	m_spellList.push_back(0);
//	m_spellList.push_back(1);
//	m_spellList.push_back(2);
//	m_spellList.push_back(3);
//	m_spellList.push_back(4);
}

void Player::cast(Uint16 spellID)
{
	if (m_currentState == DEAD)
	{
		printf("Cannot cast spells when dead!\n");
		return;
	}

	// TODO: temporary, since we're not actually passing in the spellID yet.
	spellID -= 1;

	// TODO: eventually we need to verify this spell id is in the player's m_spellList...
	// TODO: offensive spells cannot be cast unless m_currentState == COMBAT

	Spell *s = SpellSystem::getSingleton().getSpell(spellID);

	// Verify entity has enough mana to cast this spell.
	// TODO: will also need to verify he has all the correct regeants, etc, needed to cast this spell
	if (m_stats.mp < s->m_manaConsumption)
	{
		printf("Insuffienct mana!\n");
		return;
	}

	// TODO: self-only spells don't need this check.
	if (m_currentTarget == NULL)
	{
		printf("No target!\n");
		return;
	}

	TypingSystem::getSingleton().invoke(this, m_currentTarget, s);
}

void Player::update()
{
	EntityBase::update();
}

NPC::NPC(const std::string &name /* = "A NPC" */)
: EntityBase(name)
{
	m_stats.hp = 100;
	m_stats.mp = 100;

	m_combatTimer = 0.0f;
}

void NPC::cast(Uint16 spellID)
{
	// TODO: temporary, since we're not actually passing in the spellID yet.
	spellID -= 1;

	// TODO: eventually we need to verify this spell id is in the player's m_spellList...

	Spell *s = SpellSystem::getSingleton().getSpell(spellID);

	// Verify entity has enough mana to cast this spell.
	// TODO: will also need to verify he has all the correct regeants, etc, needed to cast this spell
	if (m_stats.mp < s->m_manaConsumption)
	{
		printf("NPC: Insuffienct mana!\n");
		return;
	}

	assert(m_currentTarget);

	SpellSystem::getSingleton().apply(s, this, m_currentTarget, 1.0);	// TODO: accuracy needs to be based on NPC stats
}

void NPC::update()
{
	EntityBase::update();

	switch (m_currentState)
	{
	case WORLD:
		{
			// TODO: NPC walking/movement algorithms here.
		}
		break;

	case COMBAT:
		{
			// TODO: for now, static is fine.. but later, different types of enemies
			//		 will attack with different speeds.
			const static float ATTACK_SPEED = 5.0f;

			// Attack?!
			if (m_combatTimer >= ATTACK_SPEED)
			{
				std::vector< EntityBase* > combatants;
				for (Uint8 i = 0; i < m_combatants.size(); ++i)
				{
					// TODO: check if confused.
					if (dynamic_cast<Player*>(m_combatants[i]))
					{
						combatants.push_back(m_combatants[i]);
					}
				}

				assert(combatants.size() >= 1);
				int randVal = rand() % combatants.size();

				m_currentTarget = combatants[randVal];
				cast(1);	// TODO: constantly casting fireball...

				m_combatTimer = 0.0f;
			}

			m_combatTimer += GlobalTimer::getSingleton().getTimeElapsed();
		}
		break;

	case DEAD:
		break;
	}
}
#include "NPC.h"

#include "Application.h"
#include "GUI.h"
#include "Inventory.h"
#include "Player.h"
#include "ProjectileManager.h"

#include <cmath>

NPC::NPC(Uint32 id, const std::string &name)
: EntityBase(id, name)
{
	m_fireDelay = 0;

	m_targetBox.w = 30;
	m_targetBox.h = 30;
	m_targetBox.x = 10;
	m_targetBox.y = 10;

	m_inventory = new Inventory(this);
}

void NPC::update(float deltaTime)
{
	EntityBase::update(deltaTime);

	if (m_alive)
	{
		// Movement.
		switch (m_behavior)
		{
		case ROAM:
			{
				// walk towards point #2.
				float x = m_pos2.x - m_pos.x;
				float y = m_pos2.y - m_pos.y;

				float len = sqrt(x * x + y * y);
				float dirX = x / len;
				float dirY = y / len;

				m_pos.x += .5f * m_movementSpeed * dirX * deltaTime;
				m_pos.y += .5f * m_movementSpeed * dirY * deltaTime;

				const static float eps = .5f;
				float dif = m_pos2.x - m_pos.x;
				if (abs(dif) < eps)
				{
					vec2<float> temp = m_pos2;
					m_pos2 = m_pos1;
					m_pos1 = temp;
				}
			}
			break;
		case STAY:
			break;
		}

		// if hostile and in range of player, shoot at him.
		if (m_combative)
		{
			vec2<float> dist = Application::getSingleton().m_player->getPos() - m_pos;
			float lenSq = length2(dist);

			static const float FIRE_DELAY = 4;

			m_fireDelay += deltaTime;

			// Shoot @ player if within range && rdy to fire.
			if (lenSq < 200 * 200 && m_fireDelay > FIRE_DELAY)
			{
				m_fireDelay = 0;

				dist = normalize(dist);

				static const float BULLET_SPEED = 100.0f;

				vec2<float> dir(m_pos.x + 25, m_pos.y + 10);

				ProjectileManager::getSingleton().addProjectile(this, dir, dist * BULLET_SPEED);
			}
		}
	}
	// Dead.
	else
	{

	}
}

void NPC::kill()
{
	EntityBase::kill();

	GUI::getSingleton().addToChat(m_name + " has been slain by " + m_recentAffector->getName() + "!");

	// Give exp to affector (player).
	m_recentAffector->setExp(m_recentAffector->getExp() + m_xp);
}
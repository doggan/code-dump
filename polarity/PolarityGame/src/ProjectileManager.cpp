#include "ProjectileManager.h"

#include "Application.h"
#include "NPC.h"
#include "GUI.h"
#include "ImageManager.h"
#include "Utilities.h"
#include "World.h"

ProjectileManager::ProjectileManager()
{
	ImageManager::getSingleton().getImage("bullet_dark.png", Projectile::m_imageDark);
	ImageManager::getSingleton().getImage("bullet_light.png", Projectile::m_imageLight);
}

void ProjectileManager::addProjectile(EntityBase *owner, const vec2<float> &pos, const vec2<float> &vel)
{
	Projectile *p = new Projectile(owner);
	p->setPosition(pos);
	p->setVelocity(vel);

	m_projectiles.push_back(p);
}

void ProjectileManager::update(float deltaTime)
{
	std::list< Projectile* >::iterator iter = m_projectiles.begin();

	while (iter != m_projectiles.end())
	{
		(*iter)->update(deltaTime);

		const vec2<float> projPos = (*iter)->getPos();

		// Remove from active projectile list.
		if ((*iter)->m_die)
		{
			delete (*iter);
			iter = m_projectiles.erase(iter);
		}
		else
		{
			// Check for collision against all active entities.
			std::vector< EntityBase* > ae = Application::getSingleton().getCurrentEntities();
			for (uint i = 0; i < ae.size(); ++i)
			{
				// 1). Don't allow entities to shoot themselves.
				// 2). Don't allow NPCs to hit other NPCs.
				// 3). Don't allow non-combative NPCs to be hit.
				// 4). Entity must be alive to be hit.
				if (ae[i] == (*iter)->m_owner ||
					(dynamic_cast<NPC*>((*iter)->m_owner) && dynamic_cast<NPC*>(ae[i])) ||
					!ae[i]->isCombative() ||
					!ae[i]->isAlive())
					continue;

				// Transform to world coordinates to do calculations.
				Rect tb = ae[i]->m_targetBox;
				tb.x += (int)ae[i]->getPos().x;
				tb.y += (int)ae[i]->getPos().y;

				// Collision w/ entity?
				if (projPos.x >= tb.x && projPos.x <= tb.x + tb.w &&
					projPos.y >= tb.y && projPos.y <= tb.y + tb.h)
				{
					// Apply damage.
					ae[i]->changeHP((*iter)->m_owner, -(*iter)->m_damage);

					// Display damage message.
					std::string txt = ae[i]->getName() + " takes " + toStr((*iter)->m_damage) +
											" damage from " + (*iter)->m_owner->getName() + ".";
					GUI::getSingleton().addToChat(txt);

					// Destroy this projectile.
					(*iter)->m_die = true;
				}
			}

			// Check for collision with environment (walls, etc).
			if (!Application::getSingleton().getCurrentWorld()->isWalkable(projPos.x, projPos.y))
				(*iter)->m_die = true;

			++iter;
		}
	}
}
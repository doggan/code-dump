#include "Projectile.h"

#include "EntityBase.h"

Image *Projectile::m_imageDark = NULL;
Image *Projectile::m_imageLight = NULL;

Projectile::Projectile(EntityBase *owner)
: m_owner(owner)
{
	m_polarity = m_owner->getPolarity();

	m_damage = 10;

	m_maxLifetime = 4;
	m_currentLifetime = 0;
	m_die = false;
}

void Projectile::update(float deltaTime)
{
	m_currentLifetime += deltaTime;
	if (m_currentLifetime > m_maxLifetime)
	{
		m_die = true;
		return;
	}

	m_pos += m_vel * deltaTime;
}
#ifndef _Projectile_H_
#define _Projectile_H_

#include "Common.h"

#include "vector_math.h"

// Forward declarations.
class EntityBase;

// TODO: should we make projectileManager a friend class of this? Then projectileManager
//			is the only entity who can actually do anything to the projectiles.
//			This adds lots of protection.

class Projectile
{
public:
		// TODO: get rid of requirement for polarity? We can query this from owner.
	Projectile(EntityBase *owner);
	~Projectile() {}

	void update(float deltaTime);

	vec2<float> getPos() { return m_pos; }
	void setPosition(const vec2<float> &pos) { m_pos = pos; }

	void setVelocity(const vec2<float> &vel) { m_vel = vel; }

	const Image *getImage()
	{
		if (m_polarity == DARK)
			return m_imageDark;
		else
			return m_imageLight;
	}

	static Image *m_imageDark, *m_imageLight;

	// TODO: hide all this
	int m_damage;

	EntityBase *m_owner;

	bool m_die;

private:
	float m_maxLifetime, m_currentLifetime;

public:
	vec2<float> m_pos;
	vec2<float> m_vel;

	POLARITY m_polarity;
};

#endif // _Projectile_H_
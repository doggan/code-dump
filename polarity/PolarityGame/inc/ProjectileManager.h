#ifndef _ProjectileManager_H_
#define _ProjectileManager_H_

#include "Singleton.h"
#include "EntityBase.h"
#include "Projectile.h"

#include <list>

class ProjectileManager : public Singleton<ProjectileManager>
{
public:
	ProjectileManager();
	~ProjectileManager() {}

	void addProjectile(EntityBase *owner, const vec2<float> &pos, const vec2<float> &vel);
	void update(float deltaTime);

	std::list< Projectile* > getProjectiles() { return m_projectiles; }

private:
	std::list< Projectile* > m_projectiles;
};

#endif // _ProjectileManager_H_
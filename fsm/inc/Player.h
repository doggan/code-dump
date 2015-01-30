#ifndef _Player_h_
#define _Player_h_

#include "Entity.h"

class Player : public Entity
{
public:
	Player(vec2 pos, float dir);
	virtual ~Player() {}

	void setTargetPos(vec2 targetPos) { m_targetPos = targetPos; }

	void display(float deltaTime);
	void update(float deltaTime);

private:
	vec2 m_velocity;
	vec2 m_targetPos;
};

#endif // _Player_h_
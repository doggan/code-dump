#ifndef _Entity_h_
#define _Entity_h_

#include "MtxLib.h"

class Entity
{
public:
	Entity(vec2 pos, float dir)
	{
		m_pos = pos;
		m_dir = dir;
	}

	virtual ~Entity() {}

	virtual void display(float deltaTime) = 0;
	virtual void update(float deltaTime);

	vec2 getHeading() {
		vec3 res(1, 0, 0);
		res = res * RotateRad(DegToRad(m_dir));
		return vec2(res.x, res.y);
	}

	vec2 getPosition() { return m_pos; }
	void setPosition(vec2 pos) { m_pos = pos; }

protected:
	vec2 m_pos;
	float m_dir;
};

#endif // _Entity_h_
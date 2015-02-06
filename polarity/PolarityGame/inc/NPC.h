#ifndef _NPC_h_
#define _NPC_h_

#include "EntityBase.h"

class NPC : public EntityBase
{
public:
	enum BEHAVIOR
	{
		STAY,
		ROAM
	};

	NPC(Uint32 id, const std::string &name = "A NPC");
	~NPC() {}

	NPC *create() const { return new NPC(*this); }

	void update(float deltaTime);

	// TODO: private
	vec2<float> m_vel;
	vec2<float> m_pos1;
	vec2<float> m_pos2;

	BEHAVIOR m_behavior;

private:
	void kill();

	float m_fireDelay;
};

#endif // _NPC_h_
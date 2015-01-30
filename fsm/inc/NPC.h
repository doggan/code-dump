#ifndef _NPC_h_
#define _NPC_h_

#include "Entity.h"

#include "AIStateMachine.h"

class AIStateMachine;

class NPC : public Entity
{
public:
	NPC(vec2 pos, float dir);
	virtual ~NPC() {}

	void display(float deltaTime);
	void update(float deltaTime);

	std::string getCurrentStateName() { return m_ai->getCurrentStateName(); }

private:
	AIStateMachine *m_ai;
};

#endif // _NPC_h_
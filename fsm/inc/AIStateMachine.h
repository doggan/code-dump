#ifndef _AIStateMachine_h_
#define _AIStateMachine_h_

#include <string>

class NPC;
class AIState;

class AIStateMachine
{
public:
	AIStateMachine();
	virtual ~AIStateMachine() {}

	void changeState(AIState *nextState);
	void update(float time);

	std::string getCurrentStateName();

private:
	AIState *m_currentState;
};

class AIState
{
public:
	AIState(NPC *npc, AIStateMachine *aism) : m_npc(npc), m_aism(aism) {}
	virtual ~AIState() {}

	virtual void onEnter() = 0;
	virtual void onUpdate(float time) = 0;
	virtual void onExit() = 0;

	virtual std::string getName() = 0;

protected:
	AIStateMachine *m_aism;
	NPC *m_npc;
};

class AIState_Chat : public AIState
{
public:
	AIState_Chat(NPC *npc, AIStateMachine *aism) : AIState(npc, aism) {}
	virtual ~AIState_Chat() {}

	void onEnter();
	void onUpdate(float time);
	void onExit();

	std::string getName() { return "CHAT"; }
};

class AIState_Walk : public AIState
{
public:
	AIState_Walk(NPC *npc, AIStateMachine *aism) : AIState(npc, aism) {}
	virtual ~AIState_Walk() {}

	void onEnter();
	void onUpdate(float time);
	void onExit();

	std::string getName() { return "WALK"; }
};

class AIState_Run : public AIState
{
public:
	AIState_Run(NPC *npc, AIStateMachine *aism) : AIState(npc, aism) {}
	virtual ~AIState_Run() {}

	void onEnter();
	void onUpdate(float time);
	void onExit();

	std::string getName() { return "RUN"; }
};

#endif // _AIStateMachine_h_
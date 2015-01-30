#include "AIStateMachine.h"
#include "NPC.h"
#include "Player.h"
#include "MtxLib.h"
#include "Application.h"

#include <cstdlib>

AIStateMachine::AIStateMachine()
{
	m_currentState = NULL;
}

void AIStateMachine::update(float time)
{
	if (m_currentState)
		m_currentState->onUpdate(time);
}

void AIStateMachine::changeState(AIState *newState)
{
	if (m_currentState)
	{
		m_currentState->onExit();
		delete m_currentState;
	}

	m_currentState = newState;
	m_currentState->onEnter();
}

std::string AIStateMachine::getCurrentStateName()
{
	return m_currentState->getName();
}

// AI_State_Idle
void AIState_Chat::onEnter()
{

}

void AIState_Chat::onUpdate(float time)
{
	vec2 heading = m_npc->getHeading();
	vec2 pos = m_npc->getPosition();

	vec2 playerPos = Application::getSingleton().getPlayer()->getPosition();

	// If within affect range of player, switch to run state.
	if (vec2(pos - playerPos).lengthSqr() <= 80 * 80)
	{
		m_aism->changeState(new AIState_Run(m_npc, m_aism));
		return;
	}

	std::list< NPC* > npcs = Application::getSingleton().getNPCs();
	for (std::list< NPC* >::iterator i = npcs.begin(); i != npcs.end(); ++i)
	{
		vec2 npcPos = (*i)->getPosition();

		// If inside affect range of any NPC, stay in idle state.
		if ((*i) != m_npc && vec2(pos - npcPos).lengthSqr() <= 40 * 40)
		{
			return;
		}
	}

	// Else switch to walk state.
	m_aism->changeState(new AIState_Walk(m_npc, m_aism));
}

void AIState_Chat::onExit()
{

}

// AI_State_Walk
void AIState_Walk::onEnter()
{
	
}

void AIState_Walk::onUpdate(float time)
{
	vec2 heading = m_npc->getHeading();
	vec2 pos = m_npc->getPosition();

	vec2 playerPos = Application::getSingleton().getPlayer()->getPosition();

	// If within affect range of player, switch to run state.
	if (vec2(pos - playerPos).lengthSqr() <= 80 * 80)
	{
		m_aism->changeState(new AIState_Run(m_npc, m_aism));
		return;
	}

	std::list< NPC* > npcs = Application::getSingleton().getNPCs();
	for (std::list< NPC* >::iterator i = npcs.begin(); i != npcs.end(); ++i)
	{
		vec2 npcPos = (*i)->getPosition();

		// If within affect range of another NPC, switch to idle state.
		float chatRoll = rand() / (RAND_MAX - 1.0f);
		float chatChance = .005f;
		if ((*i) != m_npc &&
			vec2(pos - npcPos).lengthSqr() <= 40 * 40 &&
			chatRoll <= chatChance)
		{
			m_aism->changeState(new AIState_Chat(m_npc, m_aism));
			return;
		}
	}

	// Else stay in walk state.
	float speed = 20.0f;
	m_npc->setPosition(heading * speed * time + pos);
}

void AIState_Walk::onExit()
{

}

// AI_State_Run
void AIState_Run::onEnter()
{
	
}

void AIState_Run::onUpdate(float time)
{
	vec2 heading = m_npc->getHeading();
	vec2 pos = m_npc->getPosition();

	vec2 playerPos = Application::getSingleton().getPlayer()->getPosition();

	// If outside the affect range of player, switch to walk state.
	if (vec2(pos - playerPos).lengthSqr() > 80 * 80)
	{
		m_aism->changeState(new AIState_Walk(m_npc, m_aism));
		return;
	}

	// Else stay in run state.
	float speed = 60.0f;
	m_npc->setPosition(heading * speed * time + pos);
}

void AIState_Run::onExit()
{
	
}
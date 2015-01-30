#include "NPC.h"
#include "2D_Rendering.h"

#include <gl/glut.h>

NPC::NPC(vec2 pos, float dir)
 : Entity(pos, dir)
{
	m_ai = new AIStateMachine();
	m_ai->changeState(new AIState_Walk(this, m_ai));
}

void NPC::display(float deltaTime)
{
	m_ai->update(deltaTime);

	glPushMatrix();

	glTranslatef(m_pos.x, m_pos.y, 0);
	glRotatef(m_dir, 0, 0, -1);
	glColor3f(0, 0, 1);

	// Draw the NPC.
	_2D::hollowTriangle(24, 20);

	// Draw the affect radius.
	_2D::hollowCircle(40);

	glPopMatrix();
}

void NPC::update(float deltaTime)
{
	Entity::update(deltaTime);
}
#include "Player.h"
#include "2D_Rendering.h"

#include <gl/glut.h>

Player::Player(vec2 pos, float dir)
: Entity(pos, dir)
{
	m_targetPos = pos;
	m_velocity.set(0, 0);
}

void Player::display(float deltaTime)
{
	glPushMatrix();

	glTranslatef(m_pos.x, m_pos.y, 0);
	glRotatef(m_dir, 0, 0, -1);
	glColor3f(1, 0, 0);

	// Draw the player.
	_2D::solidTriangle(24, 20);

	// Draw the affect radius.
	_2D::hollowCircle(80);

	glPopMatrix();
}

void Player::update(float deltaTime)
{
	Entity::update(deltaTime);

	// "Arrival" code
	vec2 toTarget = m_targetPos - m_pos;
	float dist = toTarget.length();

	vec2 steeringForce(0, 0);

	const float maxSpeed = 255.0f;

	if (dist > 0)
	{
		const float tweaker = .8f;

		float speed = dist / (2 * tweaker);
		speed = (speed < maxSpeed) ? (speed) : (maxSpeed);

		vec2 desiredVel = toTarget * speed / dist;

		steeringForce = desiredVel - m_velocity;
	}

	float mass = 0.5f;
	vec2 accel = steeringForce / mass;

	m_velocity += accel * deltaTime;
	if (m_velocity.lengthSqr() > maxSpeed * maxSpeed)
	{
		float alpha = (maxSpeed * maxSpeed) / m_velocity.lengthSqr();
		m_velocity.x *= alpha;
		m_velocity.y *= alpha;
	}

	m_pos += m_velocity * deltaTime;

	if (m_velocity.lengthSqr() > 1)
	{
		float res = acos(DotProduct(Normalized(m_velocity), vec2(1, 0)));

		if (m_velocity.y < 0.0f)
			m_dir = RadToDeg(res);
		else
			m_dir = -RadToDeg(res);
	}
}
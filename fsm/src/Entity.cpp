#include "Entity.h"
#include "Application.h"

void Entity::update(float deltaTime)
{
	// Wrap around screen.
	if (m_pos.x < 0)
		m_pos.x = WINDOW_WIDTH - 1;
	else if (m_pos.x >= WINDOW_WIDTH)
		m_pos.x = 0;
	if (m_pos.y < 0)
		m_pos.y = WINDOW_HEIGHT - 1;
	else if (m_pos.y >= WINDOW_HEIGHT)
		m_pos.y = 0;
}
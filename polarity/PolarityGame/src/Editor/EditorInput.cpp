#ifdef EDITOR

#include "EditorInput.h"

void EditorInput::update()
{
	// if button is down, change it to HELD
	// if button is up, change it to INACTIVE

	// Mouse.
	{
		std::map<int, KEY_STATE>::iterator iter;
		for(iter = m_buttonStates.begin(); iter != m_buttonStates.end(); ++iter)
		{
			// From down to held.
			if (iter->second == KEY_DOWN)
			{
				iter->second = KEY_HELD;
			}
			// From up to inactive.
			else if (iter->second == KEY_UP)
			{
				iter->second = KEY_INACTIVE;
			}
		}
	}

	// Keyboard.
	{
		std::map<int, KEY_STATE>::iterator iter;
		for(iter = m_keyStates.begin(); iter != m_keyStates.end(); ++iter)
		{
			// From down to held.
			if (iter->second == KEY_DOWN)
			{
				iter->second = KEY_HELD;
			}
			// From up to inactive.
			else if (iter->second == KEY_UP)
			{
				iter->second = KEY_INACTIVE;
			}
		}
	}
}

void EditorInput::update(int key, KEY_STATE state)
{
	// Avoid overwriting a currently KEY_HELD with a KEY_DOWN, so
	// we can decern the difference. wxWidgets fires the same
	// event regardless, but we want to know the difference!
	if (state == KEY_DOWN &&
		m_keyStates.find(key) != m_keyStates.end())
	{
		if (m_keyStates[key] == KEY_HELD)
			return;
	}

	m_keyStates[key] = state;
}

void EditorInput::update(int button, KEY_STATE state, vec2<uint> pos)
{
	m_buttonStates[button] = state;
	m_mousePos = pos;
}

void EditorInput::update(vec2<uint> pos)
{
	m_mousePos = pos;
}

#endif
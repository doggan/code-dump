#include "Input.h"

#include "Common.h"
#include "GUI.h"

#include <vector>

bool Input::update()
{
	SDL_Event event;
	std::vector< SDLKey > changedKeys;
	std::vector< int > changedButtons;

	while (SDL_PollEvent(&event))
	{
		GUI::getSingleton().pushInput(event);

		if (event.type == SDL_QUIT)
		{
			return false;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return false;

			m_keyStates[event.key.keysym.sym] = KEY_DOWN;
			changedKeys.push_back(event.key.keysym.sym);
		}
		else if (event.type == SDL_KEYUP)
		{
			m_keyStates[event.key.keysym.sym] = KEY_UP;
			changedKeys.push_back(event.key.keysym.sym);
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			m_mousePos.x = event.motion.x;
			m_mousePos.y = event.motion.y;
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			m_buttonStates[event.button.button] = KEY_DOWN;
			changedButtons.push_back(event.button.button);
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			m_buttonStates[event.button.button] = KEY_UP;
			changedButtons.push_back(event.button.button);
		}
	}

	// Keys.
	{
		std::map<SDLKey, KEY_STATE>::iterator iter;
		for(iter = m_keyStates.begin(); iter != m_keyStates.end(); ++iter)
		{
			// From up to inactive.
			if (iter->second == KEY_UP)
			{
				bool keyFound = false;
				for(uint i = 0; i < changedKeys.size(); ++i)
				{
					if (changedKeys[i] == iter->first)
					{
						keyFound = true;
						break;
					}
				}

				if (!keyFound)
				{
					iter->second = KEY_INACTIVE;
				}
			}
			// From down to held.
			else if (iter->second == KEY_DOWN)
			{
				bool keyFound = false;
				for (uint i = 0; i < changedKeys.size(); ++i)
				{
					if (changedKeys[i] == iter->first)
					{
						keyFound = true;
						break;
					}
				}

				if (!keyFound)
				{
					iter->second = KEY_HELD;
				}
			}
		}
	}

	// Mouse.
	{
		std::map<int, KEY_STATE>::iterator iter;
		for(iter = m_buttonStates.begin(); iter != m_buttonStates.end(); ++iter)
		{
			// From up to inactive.
			if (iter->second == KEY_UP)
			{
				bool keyFound = false;
				for(uint i = 0; i < changedButtons.size(); ++i)
				{
					if (changedButtons[i] == iter->first)
					{
						keyFound = true;
						break;
					}
				}

				if (!keyFound)
				{
					iter->second = KEY_INACTIVE;
				}
			}
			// From down to held.
			else if (iter->second == KEY_DOWN)
			{
				bool keyFound = false;
				for (uint i = 0; i < changedButtons.size(); ++i)
				{
					if (changedButtons[i] == iter->first)
					{
						keyFound = true;
						break;
					}
				}

				if (!keyFound)
				{
					iter->second = KEY_HELD;
				}
			}
		}
	}

	return true;
}
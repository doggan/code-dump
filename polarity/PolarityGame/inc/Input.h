#ifndef _Input_h_
#define _Input_h_

#include "Singleton.h"
#include "vector_math.h"

#include <map>

#include "SDL.h"

class Input : public Singleton< Input >
{
public:
	Input() {}
	~Input() {}

	enum KEY_STATE
	{
		KEY_INACTIVE,
		KEY_UP,
		KEY_DOWN,
		KEY_HELD
	};

	bool update();

	vec2<int> getMousePos() { return m_mousePos; }

	bool isKeyUp(SDLKey key) { return m_keyStates[key] == KEY_UP; }
	bool isKeyDown(SDLKey key) { return m_keyStates[key] == KEY_DOWN; }
	bool isKeyHeld(SDLKey key) { return m_keyStates[key] == KEY_HELD || isKeyDown(key); }

	bool isMouseButtonUp(int button) { return m_buttonStates[button] == KEY_UP; }
	bool isMouseButtonDown(int button) { return m_buttonStates[button] == KEY_DOWN; }
	bool isMouseButtonHeld(int button) { return m_buttonStates[button] == KEY_HELD || isMouseButtonDown(button); }

private:
	std::map<SDLKey, KEY_STATE> m_keyStates;
	std::map<int, KEY_STATE> m_buttonStates;

	vec2<int> m_mousePos;
};

#endif // _Input_h_
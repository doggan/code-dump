#ifdef EDITOR

#ifndef _EditorInput_h_
#define _EditorInput_h_

#include "Common.h"
#include "Singleton.h"

#include "vector_math.h"

#include "wx/wx.h"

#include <map>

class EditorInput : public Singleton< EditorInput >
{
public:
	EditorInput() {}
	~EditorInput() {}

	enum KEY_STATE
	{
		KEY_INACTIVE,
		KEY_UP,
		KEY_DOWN,
		KEY_HELD
	};

	void update();
	void update(int key, KEY_STATE state);
	void update(int button, KEY_STATE state, vec2<uint> pos);
	void update(vec2<uint> pos);

	vec2<uint> getMousePos() { return m_mousePos; }

	bool isKeyUp(int key) { return m_keyStates[key] == KEY_UP; }
	bool isKeyDown(int key) { return m_keyStates[key] == KEY_DOWN; }
	bool isKeyHeld(int key) { return m_keyStates[key] == KEY_HELD || isKeyDown(key); }

	bool isMouseButtonUp(int button) { return m_buttonStates[button] == KEY_UP; }
	bool isMouseButtonDown(int button) { return m_buttonStates[button] == KEY_DOWN; }
	bool isMouseButtonHeld(int button) { return m_buttonStates[button] == KEY_HELD || isMouseButtonDown(button); }

private:
	std::map<int, KEY_STATE> m_keyStates;
	std::map<int, KEY_STATE> m_buttonStates;

	vec2<uint> m_mousePos;
};

#endif // _EditorInput_h_

#endif
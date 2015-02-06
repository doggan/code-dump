#ifdef EDITOR

#ifndef _MainFrame_h_
#define _MainFrame_h_

#include "Common.h"
#include "vector_math.h"

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include <vector>
/*
TODO:

- npc saving/editing system
	X auto-id generating
	- name
	- stats editing
	- loot editing
- loot/object editing

- script system (Lua!)
	- How triggered?
		- Time based
		- Proximity based (step on a given tile).
		- 'Event' Based (when an enemy is killed, when a world is entered/exited....).
	- Stuff scripts need to do:
		- create new NPCs
		- create effects (particles, flying sprites, etc)
	- Can be tied to: world (tiles, onEnter, onLeave...), NPCs (onDeath, ...)

- Debugging system (debug overlay + macros/colors).

- spell editing
	- would be nice to be able to create gestures in the editor, and save out to the
		spells.xml

- fit all of the above into undo/redo system
- more efficient save system (only write out changes)
*/

// Forward declarations.
class ContentBrowser;
class EventTimer;
class LayerPanel;
class GLCanvas;
class Timer;
class World;

class MainFrame: public wxFrame
{
	DECLARE_EVENT_TABLE();

public:
	MainFrame(
		wxWindow *parent,
		const wxString &title,
		wxPoint pos,
		wxSize size,
		long style = wxDEFAULT_FRAME_STYLE
		);

	~MainFrame();

	void update();

private:
	GLCanvas *m_canvas;
	EventTimer *m_eventTimer; // Used to create a 'render loop' for the OpenGL context.
	Timer *m_timer; // Used for all game-related time.

	LayerPanel *m_layerPanel;
	ContentBrowser *m_contentBrowser;

	void buildMenu();

	// Called when the X is clicked.
	void onExitX(wxCloseEvent &event);

	void onNew(wxCommandEvent &event);
	void onOpen(wxCommandEvent &event);
	void onSave(wxCommandEvent &event);
	void onExit(wxCommandEvent &event);

	void onDrawGrid(wxCommandEvent &event);

	void preFrame(float deltaTime);
	void frame(float deltaTime);

	void drawSelectedTiles();
	vec2<uint> m_selectedTiles;

	void drawGrid();

	World *m_world;
	Image *m_collisionLayer;
	vec2<float> m_cameraOffset;
};

namespace _MainFrame
{
	enum
	{
		ID_NEW = 1,
		ID_OPEN,
		ID_SAVE,
		ID_SAVE_AS,
		ID_EXIT,

		ID_UNDO,
		ID_REDO,

		ID_DRAW_GRID,

		ID_ABOUT,
		ID_HELP_INDEX,
	};
}

/* TODO: Scroll functionality for the OpenGL window.
:     Yes I think it's better to use a sizer and place your wxGLCanvas into the sizer...

Personnaly I've tried :

wxFrame
wxSizer
wxScrolledWindow
wxSizer
wxGLCanvas <-- give him a fixed size for exemple wxSize(1000,1000)

Yes - it works , thank you very much for your help.
*/

/* A wrapper for the OpenGL window. Nothing should be handled here.
* Pass everything to the MainFrame.
*/
class GLCanvas : public wxGLCanvas
{
	DECLARE_EVENT_TABLE();

public:
	GLCanvas(wxWindow *parent, wxSize size, int *args);

	void init();

private:
	void onMouseMoved(wxMouseEvent &event);
	void onLeftMouseDown(wxMouseEvent &event);
	void onLeftMouseUp(wxMouseEvent &event);
	void onRightMouseDown(wxMouseEvent &event);
	void onRightMouseUp(wxMouseEvent &event);
	void onKeyPressed(wxKeyEvent &event);
	void onKeyReleased(wxKeyEvent &event);

	wxGLContext *m_context;
};

class EventTimer : public wxTimer
{
private:
	MainFrame *m_frame;

public:
	EventTimer(MainFrame *frame) { m_frame = frame; }

	void Notify() { m_frame->update(); }
};


#endif // _MainFrame_h_

#endif
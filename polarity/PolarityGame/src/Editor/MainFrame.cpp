#ifdef EDITOR

#include "MainFrame.h"

#include "ContentBrowser.h"
#include "EditorData.h"
#include "EditorInput.h"
#include "LayerPanel.h"
#include "NewWorldDialog.h"

#include "ImageManager.h"
#include "NPCManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "World.h"

using namespace _MainFrame;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)

	EVT_CLOSE(MainFrame::onExitX)

	EVT_MENU(ID_NEW, MainFrame::onNew)
	EVT_MENU(ID_OPEN, MainFrame::onOpen)
	EVT_MENU(ID_SAVE, MainFrame::onSave)
	//EVT_MENU(ID_SAVE_AS, MainFrame::onSaveAs)
	EVT_MENU(ID_EXIT, MainFrame::onExit)

	EVT_MENU(ID_DRAW_GRID, MainFrame::onDrawGrid)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)

	EVT_MOTION(GLCanvas::onMouseMoved)
	EVT_LEFT_DOWN(GLCanvas::onLeftMouseDown)
	EVT_LEFT_UP(GLCanvas::onLeftMouseUp)
	EVT_RIGHT_DOWN(GLCanvas::onRightMouseDown)
	EVT_RIGHT_UP(GLCanvas::onRightMouseUp)
	EVT_KEY_DOWN(GLCanvas::onKeyPressed)
	EVT_KEY_UP(GLCanvas::onKeyReleased)

END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent, const wxString &title, wxPoint pos, wxSize size, long style)
: wxFrame(parent, wxID_ANY, title, pos, size, style)
{
	new EditorData();

	// MainFrame components.
	{
		buildMenu();

		// TODO: i will need to create my own derived scroll window class to manage this.
		//			On scrolling, it should change the camera coordinates.
		wxScrolledWindow *glScroll = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(600, 500));
		glScroll->SetScrollbars(10, 10, 500, 500);

		int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, SCREEN_BPP, 0};
		m_canvas = new GLCanvas(glScroll, wxSize(size.x * .80f, size.y), args);

		m_layerPanel = new LayerPanel(this);

		wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
		topSizer->Add(glScroll, 0, wxEXPAND);
		topSizer->Add(m_layerPanel, 1, wxEXPAND);

		SetSizerAndFit(topSizer);
		SetAutoLayout(true);
		Show(true);

		// This needs to be called after Show().
		m_canvas->init();
	}

	// Editor components.
	{
		m_contentBrowser = new ContentBrowser(this, "Content Browser", wxDefaultPosition, wxSize(500, 500));
	}

	// Core components.
	{
		// Initialize the various modules.
		new ImageManager();
		new Renderer();
		new EditorInput();
		m_timer = new Timer();
		m_timer->start();

		m_eventTimer = new EventTimer(this);
		m_eventTimer->Start(10);
	}

	// Application initialization.
	{
		m_world = NULL;

		ImageManager::getSingleton().getImage("editor/collisionLayer.png", m_collisionLayer);
		
		m_cameraOffset = vec2<float>(0, 0);
	}

	// Game components.
	{
		new NPCManager();
	}

	
	// TODO:
	//// NEW NPC CREATION!
	//NPC *n = new NPC(NPCManager::getSingleton().newID(), "");

	//// fill in data...
	//// OK clicked.
	//NPCManager::getSingleton().addObject(n);
	//delete n;

	//n = new NPC(NPCManager::getSingleton().newID(), "");
	//NPCManager::getSingleton().addObject(n);
}

MainFrame::~MainFrame()
{
	if (m_world)
		delete m_world;

	delete EditorData::getSingletonPtr();

	delete NPCManager::getSingletonPtr();

	m_eventTimer->Stop();

	m_timer->stop();
	delete m_timer;
	delete EditorInput::getSingletonPtr();
	delete Renderer::getSingletonPtr();
	delete ImageManager::getSingletonPtr();
}

void MainFrame::buildMenu()
{
	// File.
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_NEW, "&New");
	menuFile->Append(ID_OPEN, "&Open");
	menuFile->AppendSeparator();
	menuFile->Append(ID_SAVE, "&Save\tCtrl+S");
//	menuFile->Append(ID_SAVE_AS, "Save &As");
	menuFile->AppendSeparator();
	menuFile->Append(ID_EXIT, "E&xit");

	// View.
	wxMenu *menuView = new wxMenu;
	menuView->Append(ID_DRAW_GRID, "Draw Grid", wxEmptyString, true);
	menuView->Check(ID_DRAW_GRID, true);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuView, "&View");

	SetMenuBar(menuBar);
}

void MainFrame::onNew(wxCommandEvent &event)
{
	NewWorldDialog newWorld(this, "New World");

	if (newWorld.ShowModal() == wxID_OK)
	{
		if (m_world)
			delete m_world;

		m_world = new World(newWorld.getName(), newWorld.getNum(), vec2<uint>(32, 32));
		EditorData::getSingleton().setWorldLoaded(true);
	}
}

void MainFrame::onOpen(wxCommandEvent &event)
{
	try
	{
		m_world = new World;
		m_world->load("data/editorTest.xml");
		EditorData::getSingleton().setWorldLoaded(true);

		/*
		std::string worldFile = wxFileSelector(
			"Open World",
			NULL,
			NULL,
			"xml",
			"World files (*.xml)|*.xml");

		if (!worldFile.empty())
		{
			m_world = new World;
			m_world->load(worldFile);
			EditorData::getSingleton().setWorldLoaded(true);
		}
		*/
	}
	catch (Exception &e)
	{
		e.messageBox();
	}
}

void MainFrame::onSave(wxCommandEvent &event)
{
	if (EditorData::getSingleton().isWorldLoaded())
	{
	//	std::string fileName = m_world->getFileName();
	//	if (!fileName.empty())
	//	{
	//		m_world->save(fileName);
			m_world->save("data/editorTestSave.xml");
	//	}
	}
}

void MainFrame::onExit(wxCommandEvent &event)
{
	Destroy();
}

void MainFrame::onExitX(wxCloseEvent &event)
{
	wxCommandEvent dummy;
	onExit(dummy);
}

void MainFrame::onDrawGrid(wxCommandEvent &event)
{
	EditorData::getSingleton().setDrawGrid(!EditorData::getSingleton().isDrawGrid());
}

void MainFrame::update()
{
	m_timer->update();

	float deltaTime = m_timer->getTimeElapsed();

	if (EditorData::getSingleton().isWorldLoaded())
	{
		preFrame(deltaTime);

		m_world->setRenderArea(m_cameraOffset, vec2<uint>(SCREEN_WIDTH, SCREEN_HEIGHT));

		frame(deltaTime);
	}

	m_canvas->SwapBuffers();

	// This needs to be done at end.
	EditorInput::getSingleton().update();
}

void MainFrame::preFrame(float deltaTime)
{
	const vec2<uint> worldPos = EditorInput::getSingleton().getMousePos() + m_cameraOffset.to<uint>();
	const vec2<uint> tilePos = m_world->worldToTileCoords(worldPos);
	const vec2<uint> numTiles = m_world->getNumberOfTiles();

	// Verify we're in boundaries of the map.
	if (tilePos.x < numTiles.x && tilePos.y < numTiles.y)
	{
		// Select the tile (visual selection).
		m_selectedTiles = worldPos;

		if (EditorInput::getSingleton().isMouseButtonDown(SDL_BUTTON_LEFT))
		{
			// Toggle collision.
			if (EditorData::getSingleton().getActiveLayer() == LAYER_COLLISION)
			{
				m_world->toggleCollision(tilePos);
			}
		}

		if (EditorInput::getSingleton().isMouseButtonHeld(SDL_BUTTON_LEFT))
		{
			// Apply the active tile to this location.
			EditorData::Tile *tile = EditorData::getSingleton().getActiveTile();
			if (tile)
			{
				m_world->setTileImage(tilePos, EditorData::getSingleton().getActiveLayer(), tile->name);
			}
		}

		if (EditorInput::getSingleton().isMouseButtonDown(SDL_BUTTON_RIGHT))
		{
			// Change active tile to this location.
			std::string name = m_world->getTileImage(tilePos, EditorData::getSingleton().getActiveLayer());
			EditorData::getSingleton().setActiveTile(EditorData::getSingleton().getTile(name));
			m_layerPanel->Refresh();
		}
	}

	// Camera.
	{
		const float PAN_SPEED = 200 * deltaTime;

		if (EditorInput::getSingleton().isKeyHeld(WXK_DOWN))
		{
			m_cameraOffset.y += PAN_SPEED;
		}
		if (EditorInput::getSingleton().isKeyHeld(WXK_UP))
		{
			m_cameraOffset.y -= PAN_SPEED;
		}
		if (EditorInput::getSingleton().isKeyHeld(WXK_RIGHT))
		{
			m_cameraOffset.x += PAN_SPEED;
		}
		if (EditorInput::getSingleton().isKeyHeld(WXK_LEFT))
		{
			m_cameraOffset.x -= PAN_SPEED;
		}

		if (m_cameraOffset.x < 0)
			m_cameraOffset.x = 0;
		if (m_cameraOffset.y < 0)
			m_cameraOffset.y = 0;
	}

	// Bring up the Content Editing window.
	if (EditorInput::getSingleton().isKeyDown(WXK_BACK))
	{
		m_contentBrowser->Show();
	}
}

void MainFrame::frame(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	// Rendering.
	{
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glLoadIdentity();

		// Camera.
		glTranslatef(-m_cameraOffset.x, -m_cameraOffset.y, 0);

		if (EditorData::getSingleton().isShowLayer(0))
			m_world->renderLayer0();
		if (EditorData::getSingleton().isShowLayer(1))
			m_world->renderLayer1();

		/*
		// NPCs.
		std::vector< EntityBase* > activeEntities = m_worlds[0]->getCurrentEntities();
		for (Uint16 i = 0; i < activeEntities.size(); ++i)
		{
		const vec2<float> pos = activeEntities[i]->getPos();
		Renderer::getSingleton().blit(activeEntities[i]->getImage(), (int)pos.x, (int)pos.y);
		}
		*/

		if (EditorData::getSingleton().isShowLayer(2))
			m_world->renderLayer2();

		if (EditorData::getSingleton().isShowCollision())
			m_world->renderCollisionLayer(m_collisionLayer);

		if (EditorData::getSingleton().isDrawGrid())
			drawGrid();

		drawSelectedTiles();

		glPopAttrib();
		glPopMatrix();
	}
}

void MainFrame::drawSelectedTiles()
{
	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 0, 0);

	vec2<uint> dim = m_world->getTileDimensions();
	vec2<uint> pos = m_selectedTiles;
	pos = (pos / dim) * dim;

	glBegin(GL_LINE_LOOP);
	glVertex2i(pos.x, pos.y);
	glVertex2i(pos.x + dim.x, pos.y);
	glVertex2i(pos.x + dim.x, pos.y + dim.y);
	glVertex2i(pos.x, pos.y + dim.y);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void MainFrame::drawGrid()
{
	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);

	vec2<uint> dim = m_world->getTileDimensions();
	vec2<uint> num = m_world->getNumberOfTiles();
	vec2<uint> end(num.y * dim.x, num.x * dim.y);

	glBegin(GL_LINES);

	// Horizontal lines.
	for (uint x = 1; x < num.x; ++x)
	{
		glVertex2i(0, x * dim.y);
		glVertex2i(end.x, x * dim.y);
	}

	// Vertical lines.
	for (uint y = 1; y < num.y; ++y)
	{
		glVertex2i(y * dim.x, 0);
		glVertex2i(y * dim.x, end.y);
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

GLCanvas::GLCanvas(wxWindow *parent, wxSize size, int *args)
: wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, size, 0, "GLCanvas")
{

}

void GLCanvas::init()
{
	m_context = new wxGLContext(this, NULL);

	SetCurrent(*m_context);
}

void GLCanvas::onMouseMoved(wxMouseEvent& event)
{
	vec2< uint > p(event.GetX(), event.GetY());
	EditorInput::getSingleton().update(p);
}

void GLCanvas::onLeftMouseDown(wxMouseEvent& event)
{
	vec2< uint > p(event.GetX(), event.GetY());
	EditorInput::getSingleton().update(SDL_BUTTON_LEFT, EditorInput::KEY_DOWN, p);
}

void GLCanvas::onLeftMouseUp(wxMouseEvent& event)
{
	vec2< uint > p(event.GetX(), event.GetY());
	EditorInput::getSingleton().update(SDL_BUTTON_LEFT, EditorInput::KEY_UP, p);
}

void GLCanvas::onRightMouseDown(wxMouseEvent &event)
{
	vec2< uint > p(event.GetX(), event.GetY());
	EditorInput::getSingleton().update(SDL_BUTTON_RIGHT, EditorInput::KEY_DOWN, p);
}

void GLCanvas::onRightMouseUp(wxMouseEvent &event)
{
	vec2< uint > p(event.GetX(), event.GetY());
	EditorInput::getSingleton().update(SDL_BUTTON_RIGHT, EditorInput::KEY_UP, p);
}

void GLCanvas::onKeyPressed(wxKeyEvent& event)
{
	EditorInput::getSingleton().update(event.GetKeyCode(), EditorInput::KEY_DOWN);
}

void GLCanvas::onKeyReleased(wxKeyEvent& event)
{
	EditorInput::getSingleton().update(event.GetKeyCode(), EditorInput::KEY_UP);
}

#endif
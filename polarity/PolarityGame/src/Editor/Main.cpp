#ifdef EDITOR

#include "Main.h"

#include "MainFrame.h"

#undef main
// TODO: get rid of _CONSOLE to remove the console, and change the linker property.
IMPLEMENT_APP_CONSOLE(MainApp)

bool MainApp::OnInit()
{
	m_width = 800;
	m_height = 600;

	m_frame = new MainFrame(NULL, "Polarity - Editor", wxDefaultPosition, wxSize(m_width, m_height));

	SetTopWindow(m_frame);

	return true;
}

#endif
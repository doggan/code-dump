#ifdef EDITOR

#ifndef _Main_h_
#define _Main_h_

#include "wx/wx.h"

#include "Common.h"

// Forward declarations.
class MainFrame;

class MainApp : public wxApp
{
private:
	bool OnInit();

	uint m_width, m_height;

	MainFrame *m_frame;
};

#endif // _Main_h_

#endif
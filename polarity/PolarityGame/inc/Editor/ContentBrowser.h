#ifdef EDITOR

#ifndef _ContentBrowser_h_
#define _ContentBrowser_h_

#include "wx/wx.h"

class ContentBrowser : public wxDialog
{
public:
	ContentBrowser(wxWindow *parent,
		const wxString &title,
		const wxPoint &pos,
		const wxSize &size);
};

#endif // _ContentBrowser_h_

#endif
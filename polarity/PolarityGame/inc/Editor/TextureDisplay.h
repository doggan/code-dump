#ifdef EDITOR

#ifndef _TextureDisplay_h_
#define _TextureDisplay_h_

#include "Common.h"

#include "EditorData.h"

#include "wx/wx.h"

#include <vector>

class TextureDisplay : public wxScrolledWindow
{
	DECLARE_EVENT_TABLE();

public:
	TextureDisplay(wxWindow *parent);
	~TextureDisplay() {}

	void loadTextures(const std::string &dir);

private:
	void onPaint(wxPaintEvent &event);
	void onLeftMouseDown(wxMouseEvent &event);
	void onScroll(wxScrollWinEvent &event);
};

class ActiveTile : public wxPanel
{
	DECLARE_EVENT_TABLE();

public:
	ActiveTile(wxWindow *parent);
	~ActiveTile() {}

private:
	void onPaint(wxPaintEvent &event);
};

#endif // _TextureDisplay_h_

#endif
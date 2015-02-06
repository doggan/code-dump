#ifdef EDITOR

#include "TextureDisplay.h"

#include "EditorData.h"

#include "Utilities.h"

#include "wx/dir.h"

BEGIN_EVENT_TABLE(TextureDisplay, wxScrolledWindow)

	EVT_PAINT(TextureDisplay::onPaint)
	EVT_LEFT_DOWN(TextureDisplay::onLeftMouseDown)
	EVT_SCROLLWIN(TextureDisplay::onScroll)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ActiveTile, wxPanel)

	EVT_PAINT(ActiveTile::onPaint)

END_EVENT_TABLE()

TextureDisplay::TextureDisplay(wxWindow *parent)
: wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER)
{
	SetScrollbars(10, 10, 20, 20);

	wxImage::AddHandler(new wxPNGHandler);

	// TODO: move to new location
	loadTextures("data/images/tiles");
}

void TextureDisplay::loadTextures(const std::string &dir)
{
	// Load all the files in the directory hierarchy.
	wxArrayString pngFiles;

	if (wxDirExists(dir.c_str()))
	{
		wxDir::GetAllFiles(dir, &pngFiles, "*.png", wxDIR_FILES);
	}

	for (uint i = 0; i < pngFiles.size(); ++i)
	{
		EditorData::Tile *t = new EditorData::Tile;
		t->img = new wxBitmap(pngFiles[i], wxBITMAP_TYPE_PNG);
		t->loc.w = t->img->GetWidth();
		t->loc.h = t->img->GetHeight();

		t->name = pngFiles[i].substr(12, pngFiles[i].Len()).c_str();	// TODO: ugh...
		switchToForwardSlashes(t->name);

		// TODO: a little hack for now, to only get the correct images.
		if (t->loc.w != 32 || t->loc.h != 32)
		{
			delete t;
			continue;
		}

		EditorData::getSingleton().addTile(t);
	}
}

void TextureDisplay::onPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	const uint perRow = 2;
	uint currentRow = 0;
	uint currentCol = 0;

	std::vector< EditorData::Tile* > tiles = EditorData::getSingleton().getTile();
	for (uint i = 0; i < tiles.size(); ++i)
	{
		EditorData::Tile *t = tiles[i];

		if (t->img && t->img->Ok())
		{
			// Transparency mask.
			t->img->SetMask(new wxMask(*t->img, wxColour(0, 255, 255)));

			wxMemoryDC memDC;
			memDC.SelectObject(*t->img);

			t->loc.x = 5 + (10 + 32) * currentCol;
			t->loc.y = 5 + (10 + 32) * currentRow;

			dc.Blit(
				t->loc.x, t->loc.y,
				tiles[i]->loc.w,
				tiles[i]->loc.h,
				&memDC,
				0, 0, wxCOPY,
				true);

			memDC.SelectObject(wxNullBitmap);
		}

		++currentCol;

		if (currentCol == perRow)
		{
			currentCol = 0;
			++currentRow;
		}
	}
}

void TextureDisplay::onLeftMouseDown(wxMouseEvent &event)
{
	EditorData::Tile *s = EditorData::getSingleton().getTile(event.GetX(), event.GetY());

	if (s)
	{
		EditorData::getSingleton().setActiveTile(s);

		// TODO: I just want to refresh the active texture widget...
		this->GetParent()->Refresh();
	}
}

void TextureDisplay::onScroll(wxScrollWinEvent &event)
{
	// TODO: not working...
//	wxPaintEvent dummy;
//	OnPaint(dummy);
//	Refresh();

	event.Skip();
}

ActiveTile::ActiveTile(wxWindow *parent)
: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(32, 32))
{

}

void ActiveTile::onPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);

	EditorData::Tile *tile = EditorData::getSingleton().getActiveTile();

	if (tile && tile->img && tile->img->Ok())
	{
		// Transparency mask.
		tile->img->SetMask(new wxMask(*tile->img, wxColour(0, 255, 255)));

		wxMemoryDC memDC;
		memDC.SelectObject(*tile->img);

		dc.Blit(
			0, 0,
			tile->img->GetWidth(), tile->img->GetHeight(),
			&memDC,
			0, 0, wxCOPY,
			true);

		memDC.SelectObject(wxNullBitmap);
	}
}

#endif
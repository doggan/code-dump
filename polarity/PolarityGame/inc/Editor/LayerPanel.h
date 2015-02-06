#ifdef EDITOR

#ifndef _LayerPanel_h_
#define _LayerPanel_h_

#include "wx/wx.h"

class LayerPanel : public wxPanel
{
	DECLARE_EVENT_TABLE();

public:
	LayerPanel(wxWindow *parent);
	~LayerPanel() {}

private:
	wxCheckBox *m_collisionShow;
	wxCheckBox *m_layerShow[3];
	wxRadioBox *m_activeLayer;

	void onShowLayer(wxCommandEvent &event);
	void onActiveLayer(wxCommandEvent &event);
};

namespace _LayerPanel
{
	enum
	{
		ID_SHOW_COLLISION,
		ID_SHOW_LAYER0,
		ID_SHOW_LAYER1,
		ID_SHOW_LAYER2,
		ID_ACTIVE_LAYER
	};
}

#endif // _LayerPanel_h_

#endif
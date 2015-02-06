#ifdef EDITOR

#include "LayerPanel.h"

#include "EditorData.h"
#include "TextureDisplay.h"

using namespace _LayerPanel;

BEGIN_EVENT_TABLE(LayerPanel, wxPanel)

	EVT_CHECKBOX(ID_SHOW_COLLISION, LayerPanel::onShowLayer)
	EVT_CHECKBOX(ID_SHOW_LAYER0, LayerPanel::onShowLayer)
	EVT_CHECKBOX(ID_SHOW_LAYER1, LayerPanel::onShowLayer)
	EVT_CHECKBOX(ID_SHOW_LAYER2, LayerPanel::onShowLayer)

	EVT_RADIOBOX(ID_ACTIVE_LAYER, LayerPanel::onActiveLayer)

END_EVENT_TABLE()

LayerPanel::LayerPanel(wxWindow *parent)
: wxPanel(parent)
{
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	m_collisionShow = new wxCheckBox(this, ID_SHOW_COLLISION, "collision");
	m_layerShow[2] = new wxCheckBox(this, ID_SHOW_LAYER2, "foreground");
	m_layerShow[1] = new wxCheckBox(this, ID_SHOW_LAYER1, "fringe");
	m_layerShow[0] = new wxCheckBox(this, ID_SHOW_LAYER0, "background");

	m_collisionShow->SetValue(true);
	m_layerShow[0]->SetValue(true);
	m_layerShow[1]->SetValue(true);
	m_layerShow[2]->SetValue(true);

	// Active Layer radio box.
	static const wxString activeLayerRadioLayout[] = {
		"collision",
		"foreground",
		"fringe",
		"background"
	};

	m_activeLayer = new wxRadioBox(this,
		ID_ACTIVE_LAYER, "Active Layer:",
		wxDefaultPosition, wxDefaultSize,
		WXSIZEOF(activeLayerRadioLayout), activeLayerRadioLayout, 0, wxVERTICAL);

	m_activeLayer->SetSelection(EditorData::getSingleton().getActiveLayer());

	topSizer->Add(m_collisionShow, 0, wxALL, 5);
	topSizer->Add(m_layerShow[2], 0, wxALL, 5);
	topSizer->Add(m_layerShow[1], 0, wxALL, 5);
	topSizer->Add(m_layerShow[0], 0, wxALL, 5);
	topSizer->Add(m_activeLayer, 0, wxALL, 5);

	topSizer->Add(new ActiveTile(this), 0, wxALL | wxCENTER, 5);
	topSizer->Add(new TextureDisplay(this), 1, wxEXPAND);

	SetSizerAndFit(topSizer);
}

void LayerPanel::onShowLayer(wxCommandEvent &event)
{
	EditorData::getSingleton().setShowCollision(m_collisionShow->GetValue());
	EditorData::getSingleton().setShowLayer(m_layerShow[0]->GetValue(), 0);
	EditorData::getSingleton().setShowLayer(m_layerShow[1]->GetValue(), 1);
	EditorData::getSingleton().setShowLayer(m_layerShow[2]->GetValue(), 2);
}

void LayerPanel::onActiveLayer(wxCommandEvent &event)
{
	EditorData::getSingleton().setActiveLayer(
		static_cast<TILE_LAYER>(m_activeLayer->GetSelection())
		);
}

#endif
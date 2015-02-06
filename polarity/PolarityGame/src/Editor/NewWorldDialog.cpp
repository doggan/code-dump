#ifdef EDITOR

#include "NewWorldDialog.h"

NewWorldDialog::NewWorldDialog(wxWindow *parent, const std::string &title)
: wxDialog(parent, wxID_ANY, title, wxDefaultPosition)
{
	m_name = new wxTextCtrl(this, wxID_ANY);

	m_height = new wxSpinCtrl(this);
	m_width = new wxSpinCtrl(this);
	m_height->SetRange(1, 100);
	m_width->SetRange(1, 100);

	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *hSizer = new wxBoxSizer(wxVERTICAL);
	hSizer->Add(new wxStaticText(this, wxID_ANY, "Height:"), 0, wxCENTER);
	hSizer->Add(m_height, 0, wxCENTER);

	wxBoxSizer *wSizer = new wxBoxSizer(wxVERTICAL);
	wSizer->Add(new wxStaticText(this, wxID_ANY, "Width:"), 0, wxCENTER);
	wSizer->Add(m_width, 0, wxCENTER);

	wxBoxSizer *dimSizer = new wxBoxSizer(wxHORIZONTAL);
	dimSizer->Add(hSizer, 0, wxCENTER | wxALL, 5);
	dimSizer->Add(wSizer, 0, wxCENTER | wxALL, 5);

	wxBoxSizer *butSizer = new wxBoxSizer(wxHORIZONTAL);
	butSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
	butSizer->Add(new wxButton(this, wxID_OK, "Ok"), 0, wxALL, 5);

	topSizer->Add(m_name, 0, wxCENTER | wxALL, 5);
	topSizer->Add(dimSizer, 0, wxCENTER);
	topSizer->Add(butSizer, 0, wxCENTER);

	SetSizerAndFit(topSizer);
}

#endif
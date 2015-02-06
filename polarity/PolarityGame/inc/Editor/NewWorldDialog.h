#ifdef EDITOR

#ifndef _NewWorldDialog_h_
#define _NewWorldDialog_h_

#include "Common.h"

#include "vector_math.h"

#include "wx/wx.h"
#include "wx/spinctrl.h"

// TODO: change this to "world dialog" class. Use it to ADD NEW world,
//			and for EDIT CURRENT world.
class NewWorldDialog : public wxDialog
{
public:
	NewWorldDialog(wxWindow *parent, const std::string &title);
	~NewWorldDialog() {}

private:
	wxTextCtrl *m_name;
	wxSpinCtrl *m_width, *m_height;

public:
	std::string getName() { return m_name->GetValue().c_str(); }
	vec2< uint > getNum() { return vec2< uint >(m_height->GetValue(), m_width->GetValue()); }
};

#endif // _NewWorldDialog_h_

#endif
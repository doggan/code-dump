#ifndef _ChatDisplay_h_
#define _ChatDisplay_h_

#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "guichan/opengl.hpp"
#include "guichan/opengl/openglsdlimageloader.hpp"

// TODO: instead, should we be INHERITING from these guichan classes?

class ChatDisplay
{
public:
	ChatDisplay(gcn::Container *top);
	virtual ~ChatDisplay();
	
	void addRow(const std::string &txt);
	
protected:
	gcn::TextBox *m_textBox;
	gcn::ScrollArea *m_scrollArea;
};
	
#endif // _ChatDisplay_h_
#include "ChatDisplay.h"

ChatDisplay::ChatDisplay(gcn::Container *top)
{
	m_textBox = new gcn::TextBox();
	m_textBox->setEditable(false);

	m_scrollArea = new gcn::ScrollArea(m_textBox, gcn::ScrollArea::SHOW_AUTO, gcn::ScrollArea::SHOW_ALWAYS);
	m_scrollArea->setSize(300, 156);
	m_scrollArea->setPosition(0, 444);
	top->add(m_scrollArea);

	m_textBox->setText("Debug text window");
}

ChatDisplay::~ChatDisplay()
{
	delete m_textBox;
	delete m_scrollArea;
}

void ChatDisplay::addRow(const std::string &txt)
{
	m_textBox->addRow(txt);

	// Makes the scroll window always stay 'down'.
	m_scrollArea->setVerticalScrollAmount(m_scrollArea->getVerticalScrollAmount() +
										  m_scrollArea->getChildrenArea().height / 8);
}

#include "InventoryDisplay.h"

#include "Inventory.h"
#include "Utilities.h"

InventoryDisplay::InventoryDisplay(gcn::Container *top, gcn::ImageFont *f)
{
	m_health = new gcn::Label("Health: -");
	m_health->setPosition(200, 0);
	m_health->setSize(200, 20);
	m_health->setFont(f);
	top->add(m_health);

	m_energy = new gcn::Label("Total Energy: -");
	m_energy->setPosition(200, 15);
	m_energy->setSize(200, 20);
	m_energy->setFont(f);
	top->add(m_energy);
	
	m_health->setVisible(false);
	m_energy->setVisible(false);
}

InventoryDisplay::~InventoryDisplay()
{
	delete m_health;
	delete m_energy;
}

void InventoryDisplay::setInventory(const Inventory *i, const Stats &stats)
{
	// TODO: display all inventory contents.
	
	
	m_health->setCaption("Health: " + toStr(stats.hp));
	m_energy->setCaption("Total Energy: " + toStr(stats.energy));
}

void InventoryDisplay::toggle()
{
	bool t = !m_health->isVisible();

	m_health->setVisible(t);
	m_energy->setVisible(t);
}
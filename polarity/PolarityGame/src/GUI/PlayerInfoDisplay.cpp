#include "PlayerInfoDisplay.h"

#include "Utilities.h"

PlayerInfoDisplay::PlayerInfoDisplay(gcn::Container *top, gcn::ImageFont *f)
{
	m_health = new gcn::Label("Health: -");
	m_health->setPosition(0, 0);
	m_health->setSize(200, 20);
	m_health->setFont(f);
	top->add(m_health);

	m_xp = new gcn::Label("Exp: -");
	m_xp->setPosition(0, 15);
	m_xp->setSize(200, 20);
	m_xp->setFont(f);
	top->add(m_xp);
	
	m_lEnergy = new gcn::Label("Light Energy: -");
	m_lEnergy->setPosition(0, 30);
	m_lEnergy->setSize(200, 20);
	m_lEnergy->setFont(f);
	top->add(m_lEnergy);
	
	m_dEnergy = new gcn::Label("Dark Energy: -");
	m_dEnergy->setPosition(0, 45);
	m_dEnergy->setSize(200, 20);
	m_dEnergy->setFont(f);
	top->add(m_dEnergy);
}

PlayerInfoDisplay::~PlayerInfoDisplay()
{
	delete m_health;
	delete m_xp;
	delete m_lEnergy;
	delete m_dEnergy;
}

void PlayerInfoDisplay::setPlayerInfo(const Stats &stats, uint xp, int lEnergy, int dEnergy)
{
	m_health->setCaption("Health: " + toStr(stats.hp));
	m_xp->setCaption("Exp: " + toStr(xp));
	m_lEnergy->setCaption("Light Energy: " + toStr(lEnergy));
	m_dEnergy->setCaption("Dark Energy: " + toStr(dEnergy));
}
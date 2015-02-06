#ifndef _Player_h_
#define _Player_h_

#include "EntityBase.h"

class Player : public EntityBase
{
public:
	Player(const std::string &name = "A Player");
	~Player() {}

	void update(float deltaTime);

	void load(const std::string &fileName);
	void save(const std::string &fileName);

protected:
	void kill();

	int m_lEnergy, m_dEnergy;

public:

	// TODO: move these to cpp file.. they're kind of long.
	void changeEnergy(int delta)
	{
		if (getPolarity() == LIGHT)
		{
			m_lEnergy += delta;
			m_dEnergy -= delta;
		}
		else
		{
			m_lEnergy -= delta;
			m_dEnergy += delta;
		}
	}

	int getEnergy()
	{
		if (getPolarity() == LIGHT)
			return m_lEnergy;
		else
			return m_dEnergy;
	}

	void switchPolarity()
	{
		if (m_polarity == DARK) m_polarity = LIGHT;
		else m_polarity = DARK;
	}
};

#endif // _Player_h_
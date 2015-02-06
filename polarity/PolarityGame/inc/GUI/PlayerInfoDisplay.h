#ifndef _PlayerInfoDisplay_h_
#define _PlayerInfoDisplay_h_

#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "guichan/opengl.hpp"
#include "guichan/opengl/openglsdlimageloader.hpp"

#include "Common.h"

/* The GUI object for the player info, which is displayed all the time (health, energy, exp).
*/
class PlayerInfoDisplay // TODO: public gcn::Container
{
public:
	PlayerInfoDisplay(gcn::Container *top, gcn::ImageFont *f);
	~PlayerInfoDisplay();
	
	void setPlayerInfo(const Stats &stats, uint xp, int lEnergy, int dEnergy);

private:
//	HealthBar; EnergyBar; ExpBar;
	gcn::Label *m_health, *m_lEnergy, *m_dEnergy, *m_xp;
};

#endif // _PlayerInfoDisplay_h_
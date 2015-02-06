#ifndef _Application_h_
#define _Application_h_

#include "Singleton.h"
#include "MtxLib.h"

#include <string>
#include <vector>

using namespace _mtxCM;

// Forward declarations.
class Camera;
class ProceduralClouds;

class Application : public Singleton<Application>
{
public:
	Application();
	virtual ~Application();

	bool init();
	void display();
	void idle();
	void reshape(int w, int h);

	void processNormalKeys(unsigned char key, int x, int y);
	void processSpecialKeys(int key, int x, int y);

private:
	void displayFPS();
	void displayTerrain();
	void displaySky();
	void generateSky();

	// Lens flare.
	void displayPrimaryFlare();
	void displayFirstHalo();
	void displaySecondHalo();
	void displayThirdHalo();
	void displayFourthHalo();
	void displayFifthHalo();

	void loadTextures();
	void loadShaders();
	char *textFileRead(char *fn);

	unsigned int m_terrainTexID;
	unsigned int m_primaryFlareID;
	unsigned int m_firstHaloID;
	unsigned int m_secondHaloID;
	unsigned int m_thirdHaloID;
	unsigned int m_fourthHaloID;
	unsigned int m_fifthHaloID;
	
	struct DomeVertex {
		float x,y,z,u, v;
	};

	DomeVertex *DomeVerts;
	int NumVerts;

	float positionSun_world[3];
	double positionSun_screen[3];

	unsigned int m_vpID, m_fpID;
	unsigned int m_programID;
	
	Camera *m_camera;

	ProceduralClouds *m_procClouds;

	enum CHANGE_PARAM
	{
		CLOUD_FORMATION,	// 1
		CLOUD_COVER,		// 2
		CLOUD_SHARPNESS,	// 3
		CLOUD_SCROLL_U,		// 4
		CLOUD_SCROLL_V,		// 5
	};

	CHANGE_PARAM m_currentParam;

	float m_aspectRatio;

	bool m_inWireframe;
};

#endif // _Application_h_
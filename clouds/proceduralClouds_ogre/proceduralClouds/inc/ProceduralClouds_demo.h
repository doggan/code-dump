#ifndef _ProceduralClouds_demo_h_
#define _ProceduralClouds_demo_h_

#include "ExampleApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "ProceduralClouds.h"

ProceduralClouds *g_pc;

class ProceduralClouds_demo_FrameListener : public ExampleFrameListener
{
private:
	enum CHANGE_PARAM
	{
		CLOUD_FORMATION,	// 1
		CLOUD_COVER,		// 2
		CLOUD_SHARPNESS,	// 3
		CLOUD_SCROLL_U,		// 4
		CLOUD_SCROLL_V,		// 5
		CLOUD_MODE			// 6
	};

	CHANGE_PARAM m_currentParam;

public:
	ProceduralClouds_demo_FrameListener(RenderWindow *win, Camera *cam)
		: ExampleFrameListener(win, cam)
	{
		OverlayManager::getSingleton().getOverlayElement("Core/DebugText")->setPosition(230.0, 0.0);
	}

	bool frameStarted(const FrameEvent &evt)
	{
		if(!ExampleFrameListener::frameStarted(evt))
			return false;

		// Text.
		mDebugText = "(1) Form. Int.: " + StringConverter::toString(g_pc->getCloudFormationUpdateInterval());
		mDebugText += "\n";
		mDebugText += "(2) Cover: " + StringConverter::toString(g_pc->getCloudCover());
		mDebugText += "\n";
		mDebugText += "(3) Sharpness: " + StringConverter::toString(g_pc->getCloudSharpness());
		mDebugText += "\n";
		mDebugText += "(4) Scroll U: " + StringConverter::toString(g_pc->getCloudScrollU());
		mDebugText += "\n";
		mDebugText += "(5) Scroll V: " + StringConverter::toString(g_pc->getCloudScrollV());
		mDebugText += "\n";
		mDebugText += "(6) Mode: " + StringConverter::toString(g_pc->getCloudMode());

		g_pc->update(evt.timeSinceLastFrame);

		return true;
	}

	bool processUnbufferedKeyInput(const FrameEvent &evt)
	{
		if (ExampleFrameListener::processUnbufferedKeyInput(evt) == false)
			return false;

		using namespace OIS;

		if (mKeyboard->isKeyDown(KC_1) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_FORMATION;
		}

		if (mKeyboard->isKeyDown(KC_2) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_COVER;
		}

		if (mKeyboard->isKeyDown(KC_3) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_SHARPNESS;
		}

		if (mKeyboard->isKeyDown(KC_4) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_SCROLL_U;
		}

		if (mKeyboard->isKeyDown(KC_5) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_SCROLL_V;
		}

		if (mKeyboard->isKeyDown(KC_6) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = CLOUD_MODE;
		}

		if (mKeyboard->isKeyDown(KC_RBRACKET) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.1;

			switch (m_currentParam)
			{
			case CLOUD_FORMATION:
				g_pc->setCloudFormationUpdateInterval(.5);
				break;

			case CLOUD_COVER:
				g_pc->setCloudCover(.01);
				break;

			case CLOUD_SHARPNESS:
				g_pc->setCloudSharpness(.001);
				break;

			case CLOUD_SCROLL_U:
				g_pc->setCloudScrollU(.001);
				break;

			case CLOUD_SCROLL_V:
				g_pc->setCloudScrollV(.001);
				break;

			case CLOUD_MODE:
				mTimeUntilNextToggle = 0.5;
				g_pc->setCloudMode(1);
				break;
			}
		}

		if (mKeyboard->isKeyDown(KC_LBRACKET) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.1;

			switch (m_currentParam)
			{
			case CLOUD_FORMATION:
				g_pc->setCloudFormationUpdateInterval(-.5);
				break;

			case CLOUD_COVER:
				g_pc->setCloudCover(-.01);
				break;

			case CLOUD_SHARPNESS:
				g_pc->setCloudSharpness(-.001);
				break;

			case CLOUD_SCROLL_U:
				g_pc->setCloudScrollU(-.001);
				break;

			case CLOUD_SCROLL_V:
				g_pc->setCloudScrollV(-.001);
				break;

			case CLOUD_MODE:
				mTimeUntilNextToggle = 0.5;
				g_pc->setCloudMode(-1);
				break;
			}
		}

		return true;
	}
};

class ProceduralClouds_demo : public ExampleApplication
{
public:
	ProceduralClouds_demo() {}
	virtual ~ProceduralClouds_demo()
	{
		if (g_pc)
			delete g_pc;
	}

private:
	void createFrameListener()
	{
		mFrameListener= new ProceduralClouds_demo_FrameListener(mWindow, mCamera);
		mRoot->addFrameListener(mFrameListener);
	}

	void createScene()
	{
		{
			// Camera position.
			mCamera->setPosition(Vector3(15.767, -181.092, 100.879));

			mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

			Light *l = mSceneMgr->createLight("MainLight");
			l->setPosition(20,80,50);

			// Define a floor plane mesh.
			Plane p;
			p.normal = Vector3::UNIT_Y;
			p.d = 200;
			MeshManager::getSingleton().createPlane("FloorPlane",
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				p,2000,2000,1,1,true,1,5,5,Vector3::UNIT_Z);

			// Create an entity (the floor).
			Entity *ent = mSceneMgr->createEntity("floor", "FloorPlane");
			ent->setMaterialName("Examples/RustySteel");
			mSceneMgr->getRootSceneNode()->attachObject(ent);
		}

		// Using the ProceduralClouds add-on:
		{
			// Allocate a new object.
			g_pc = new ProceduralClouds;

			// Initialize module.
			if (!g_pc->init(20.0, .7, .01, .005, 0.0))
				exit(-1);

			// Get the material.
			const String myCloudMatName = g_pc->getMaterialName();

			// Sky dome creation. Set the material.
			mSceneMgr->setSkyDome(true, myCloudMatName, 2, 1, 3250);

			mSceneMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox");

			// .. Or a sky plane.
//			Plane plane;
//			plane.d = 10;
//			plane.normal = -Vector3::UNIT_Y;
//			mSceneMgr->setSkyPlane(true, plane, myCloudMatName, 1, 1);

			// We can also map the sky material to random objects~
			Entity *head = mSceneMgr->createEntity("ogreHead", "ogrehead.mesh");
			head->setMaterialName(g_pc->getMaterialName());
			SceneNode *headSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			headSN->attachObject(head);
			headSN->setPosition(Vector3(100, -170, 0));
		}
	}
};

#endif // _ProceduralClouds_demo_h_
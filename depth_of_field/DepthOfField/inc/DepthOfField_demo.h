#ifndef _DepthOfField_demo_h_
#define _DepthOfField_demo_h_

#include "ExampleApplication.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "DepthOfField.h"
#include <deque>

DepthOfField *g_dof;
Overlay *g_depthOverlay;

class DepthOfField_demo_FrameListener : public ExampleFrameListener
{
private:
	enum CHANGE_PARAM
	{
		NEAR_P0,	// 1
		NEAR_P1,	// 2
		FAR_P0,		// 3
		FAR_P1		// 4
	};

	CHANGE_PARAM m_currentParam;

public:
	DepthOfField_demo_FrameListener(RenderWindow *win, Camera *cam, SceneNode *sn, Entity *ent, std::deque<Vector3> &walk)
		: ExampleFrameListener(win, cam, false, false), mNode(sn), mEntity(ent), mWalkList(walk)
		, mSpinny(0)
		, mSpinnyHead(0)
		, mBillboard(0)
		, mSmoke(0)
		, DOF_MAXIMUM_CHANGE(20.0f)
		, DOF_MINIMUM_CHANGE(0.5f)
		, mInputMode(0)
		, mTargetFocalDistance(0)
	{
		//setting variables for ogre head movement
		mWalkSpeed = 35.0f;
		mDirection1 = mWalkList[1]- mWalkList[0];
		mDirection2 = mWalkList[2]- mWalkList[1];
		mDirection3 = mWalkList[3]- mWalkList[2];
		mDirection4 = mWalkList[0]- mWalkList[3];
		
		mChangeInValue = DOF_MINIMUM_CHANGE;

		OverlayManager::getSingleton().getOverlayElement("Core/DebugText")->setPosition(230.0, -10.0);
	}

	/* This function is called to start the object moving to the next position
	in mWalkList.
	*/
	bool nextLocation()
	{
		return true;
	} // nextLocation()

	void setSpinningKnot(SceneNode* node) 
	{ 
		mSpinny = node; 
	}

	void setSpinningHead(SceneNode* node) 
	{ 
		mSpinnyHead = node; 
	}

	void setBillboard(SceneNode* node) 
	{ 
		mBillboard = node; 
	}

	void setSmoke(SceneNode* node) 
	{ 
		mSmoke = node; 
	}

	bool frameStarted(const FrameEvent &evt)
	{
		if(!ExampleFrameListener::frameStarted(evt))
			return false;

		// moving ogre heads
		//nextLocation();
		//else
		//{
		//	Real move = mWalkSpeed * evt.timeSinceLastFrame;
		//	mDistance -= move;

		// Debug text.
		if (g_dof)
		{
			mDebugText = "(1) Near P0.: " + StringConverter::toString(g_dof->getNearBlurP0());
			mDebugText += "\n";
			mDebugText += "(2) Near P1: " + StringConverter::toString(g_dof->getNearBlurP1());
			mDebugText += "\n";
			mDebugText += "(3) Far P0: " + StringConverter::toString(g_dof->getFarBlurP0());
			mDebugText += "\n";
			mDebugText += "(4) Far P1: " + StringConverter::toString(g_dof->getFarBlurP1());
			mDebugText += "\n";
			mDebugText += "(5) DoF?: " + StringConverter::toString(g_dof->getEnabled());
			mDebugText += "\n";
			mDebugText += "(6) DoF Near?: " + StringConverter::toString(g_dof->getEnabledNearBlur());
			mDebugText += "\n";
			mDebugText += "(7) DoF Far?: " + StringConverter::toString(g_dof->getEnabledFarBlur());
			mDebugText += "\n";
		}

		if (mSpinny)
			mSpinny->yaw(Ogre::Degree(10 * evt.timeSinceLastFrame));
		if (mSpinnyHead)
			mSpinnyHead->yaw(Ogre::Degree(10 * evt.timeSinceLastFrame));

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
			m_currentParam = NEAR_P0;
		}

		if (mKeyboard->isKeyDown(KC_2) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = NEAR_P1;
		}

		if (mKeyboard->isKeyDown(KC_3) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = FAR_P0;
		}

		if (mKeyboard->isKeyDown(KC_4) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			m_currentParam = FAR_P1;
		}

		// Toggle DoF effect on/off.
		if (mKeyboard->isKeyDown(KC_5) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			g_dof->setEnabled(!g_dof->getEnabled());
		}

		// Toggle 'near' DoF effect on/off.
		if (mKeyboard->isKeyDown(KC_6) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			g_dof->setEnabledNearBlur(!g_dof->getEnabledNearBlur());
		}

		// Toggle 'far' DoF effect on/off.
		if (mKeyboard->isKeyDown(KC_7) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			g_dof->setEnabledFarBlur(!g_dof->getEnabledFarBlur());
		}

		// Toggle depth texture display.
		if (mKeyboard->isKeyDown(KC_8) && mTimeUntilNextToggle <= 0)
		{
			mTimeUntilNextToggle = 0.5;
			static bool showDepthWindow = true;
			if (showDepthWindow)
				g_depthOverlay->show();
			else
				g_depthOverlay->hide();
			showDepthWindow = !showDepthWindow;
		}

		if (g_dof)
		{
			if (mKeyboard->isKeyDown(KC_RBRACKET) && mTimeUntilNextToggle <= 0)
			{

				//(mTimeUntilNextToggle <= 0.02)?(mTimeUntilNextToggle=0.02):(mTimeUntilNextToggle=mTimeUntilNextToggle-mTimeUntilNextToggle/20);
				mTimeUntilNextToggle = 0.02f;
				if(mInputMode == KC_RBRACKET)
				{
					mChangeInValue *= 1.25f;
					if( mChangeInValue >= DOF_MAXIMUM_CHANGE)
					{
						mChangeInValue = DOF_MAXIMUM_CHANGE;
					}
				}
				else
				{
					mInputMode = KC_RBRACKET;
					mChangeInValue = DOF_MINIMUM_CHANGE;
				}

				switch (m_currentParam)
				{
				case NEAR_P0:
					g_dof->setNearBlurP0(g_dof->getNearBlurP0() + mChangeInValue);
					break;

				case NEAR_P1:
					g_dof->setNearBlurP1(g_dof->getNearBlurP1() + mChangeInValue);
					break;

				case FAR_P0:
					g_dof->setFarBlurP0(g_dof->getFarBlurP0() + mChangeInValue);
					break;

				case FAR_P1:
					g_dof->setFarBlurP1(g_dof->getFarBlurP1() + mChangeInValue);
					break;
				}
			}

			if (mKeyboard->isKeyDown(KC_LBRACKET) && mTimeUntilNextToggle <= 0)
			{
				//(mTimeUntilNextToggle <= 0.02)?(mTimeUntilNextToggle=0.02):(mTimeUntilNextToggle= mTimeUntilNextToggle-mTimeUntilNextToggle/20);
				mTimeUntilNextToggle = 0.02f;
				if(mInputMode == KC_LBRACKET)
				{
					mChangeInValue *= 1.25f;
					if( mChangeInValue >= DOF_MAXIMUM_CHANGE)
					{
						mChangeInValue = DOF_MAXIMUM_CHANGE;
					}
				}
				else
				{
					mInputMode = KC_LBRACKET;
					mChangeInValue = DOF_MINIMUM_CHANGE;
				}

				switch (m_currentParam)
				{
				case NEAR_P0:
					g_dof->setNearBlurP0(g_dof->getNearBlurP0() - mChangeInValue);
					break;

				case NEAR_P1:
					g_dof->setNearBlurP1(g_dof->getNearBlurP1() - mChangeInValue);
					break;

				case FAR_P0:
					g_dof->setFarBlurP0(g_dof->getFarBlurP0() - mChangeInValue);
					break;

				case FAR_P1:
					g_dof->setFarBlurP1(g_dof->getFarBlurP1() - mChangeInValue);
					break;
				}
			}

			if(!mKeyboard->isKeyDown(KC_LBRACKET) || !mKeyboard->isKeyDown(KC_RBRACKET))
			{
				//mChangeInValue = DOF_MINIMUM_CHANGE;
			}
		}

		return true;
	}
	protected:
		Real mDistance;                  // The distance the object has left to travel
		Vector3 mDirection1;              // The direction the object is moving
		Vector3 mDirection2;              // The direction the object is moving
		Vector3 mDirection3;              // The direction the object is moving
		Vector3 mDirection4;              // The direction the object is moving
		Vector3 mDestination;            // The destination the object is moving towards

		AnimationState *mAnimationState; // The current animation state of the object

		Entity *mEntity;                 // The Entity we are animating
		SceneNode *mNode;                // The SceneNode that the Entity is attached to
		std::deque<Vector3> mWalkList;   // The list of points we are walking to

		Real mWalkSpeed;                 // The speed at which the object is moving

		Ogre::SceneNode* mSpinny;
		Ogre::SceneNode* mSpinnyHead;
		Ogre::SceneNode* mBillboard;
		Ogre::SceneNode* mSmoke;

		Real mChangeInValue;
		int mInputMode;
		const Real DOF_MINIMUM_CHANGE;
		const Real DOF_MAXIMUM_CHANGE;

		Real mTargetFocalDistance;
};

class DepthOfField_demo : public ExampleApplication
{
public:
	DepthOfField_demo() {}

	virtual ~DepthOfField_demo()
	{
		if (g_dof)
			delete g_dof;
	}

protected:
	Entity *mEntity;                // The entity of the object we are animating
	SceneNode *mNode;               // The SceneNode of the object we are moving
	std::deque<Vector3> mWalkList;  // A deque containing the waypoints
	Ogre::SceneNode * mSpinny;
	Ogre::SceneNode * mSpinnyHead;
	Ogre::SceneNode * mBillboard;
	Ogre::SceneNode * mSmoke;
	DepthOfField_demo_FrameListener *mFrameListener;

private:
	void createFrameListener()
	{
		mFrameListener= new DepthOfField_demo_FrameListener(mWindow, mCamera, mNode, mEntity, mWalkList);
		mFrameListener->setSpinningKnot(mSpinny);
		mFrameListener->setSpinningHead(mSpinnyHead);
		//mFrameListener->setBillboard(mBillboard);
		//mFrameListener->setSmoke(mSmoke);
		mRoot->addFrameListener(mFrameListener);		
	}

	void createScene()
	{
		// Advance scene.
		{
			// Lighting.
			mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
			Light *light = mSceneMgr->createLight("MainLight");
			light->setType(Light::LT_DIRECTIONAL);
			Vector3 dir(1, -1, -0.5);
			dir.normalise();
			light->setDirection(dir);

			// Floor.
			Plane plane(Vector3::UNIT_Y, 0.0);
			MeshManager::getSingleton().createPlane(
				"FloorPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
				plane, 200000, 200000, 20, 20, true, 1, 500, 500, Vector3::UNIT_Z);

			Entity* entity = mSceneMgr->createEntity("floor", "FloorPlane");
			entity->setMaterialName("Examples/RustySteel");
			mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entity);

			Ogre::Entity* pEnt;


			// House
			pEnt = mSceneMgr->createEntity( "2", "tudorhouse.mesh" );
			Ogre::SceneNode* n2 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(-200, 550, -300));
			n2->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "1", "tudorhouse.mesh" );
			Ogre::SceneNode* n1 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(600, 550, -200));
			n1->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "3", "tudorhouse.mesh" );
			Ogre::SceneNode* n3 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(1400, 550, -000));
			n3->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "4", "tudorhouse.mesh" );
			Ogre::SceneNode* n4 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(2150, 550, 300));
			n4->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "5", "tudorhouse.mesh" );
			Ogre::SceneNode* n5 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(2900, 550, 650));
			n5->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "6", "tudorhouse.mesh" );
			Ogre::SceneNode* n6 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(3750, 550, 1100));
			n6->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "7", "tudorhouse.mesh" );
			Ogre::SceneNode* n7 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(4700, 550, 1650));
			n7->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "8", "tudorhouse.mesh" );
			Ogre::SceneNode* n8 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(5700, 550, 2450));
			n8->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "9", "tudorhouse.mesh" );
			Ogre::SceneNode* n9 = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(6800, 550, 3200));
			n9->attachObject( pEnt );

			pEnt = mSceneMgr->createEntity( "10", "knot.mesh" );
			mSpinny = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(300.0, 100.0, 300.0));
			mSpinny->attachObject( pEnt );
			pEnt->setMaterialName("Examples/MorningCubeMap");

			pEnt = mSceneMgr->createEntity("11", "ogrehead.mesh");
			mSpinnyHead = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(1500.0, 74.0, 450.0));
			mSpinnyHead->attachObject(pEnt);
			mSpinnyHead->scale(3.0,3.0,3.0);
			
			////Ashok - billboard
			//mBillboard = mSceneMgr->getRootSceneNode()->createChildSceneNode();
			//BillboardSet* mySet = mSceneMgr->createBillboardSet("dofBillboardset",1);
			//Billboard* myBillboard = mySet->createBillboard(Vector3(1000, 74, 500));
			//mySet->setVisible(true);
			//mySet->setMaterialName("dofbillboard");
			//mBillboard->attachObject(mySet);
			//mBillboard->scale(3.0,3.0,3.0);

			//// Smoke
			//ParticleSystem* pSys2 = mSceneMgr->createParticleSystem("smoke", "Examples/Smoke");
			//mSmoke = mSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3(4000,0,1800));
			//mSmoke->attachObject(pSys2);

			mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");

			// Ogre heads.
			pEnt = mSceneMgr->createEntity("head0", "ogrehead.mesh");
			//SceneNode *headSN = mSceneMgr->createSceneNode("snhead0");
			//headSN->attachObject(pEnt);
			SceneNode *sn = mSceneMgr->getRootSceneNode()->createChildSceneNode( "snhead0",
				Vector3(0.0, 174.0, 200.0));
			//sn->attachObject(pEnt);
			sn->yaw(Ogre::Degree(45+90));
			// creating animation - for head 1
			SceneNode *pSNHead1Animation = mSceneMgr->getRootSceneNode()->createChildSceneNode("head1Animation");
			pSNHead1Animation->attachObject(pEnt);
			//pSNFlyThrough->attachObject(mCamera);
			//mCamera->setAutoTracking(true, pSNFlyThrough);
			// Create animation for head1
			Animation* pAnimHead1= mSceneMgr->createAnimation("head1AnimationTrack", 5);
			// Spline it for nice curves
			pAnimHead1->setInterpolationMode(Animation::IM_SPLINE);
			// Create a track to animate the object
			NodeAnimationTrack* track = pAnimHead1->createNodeTrack(0, pSNHead1Animation);
			// Setup key frames
			TransformKeyFrame* key = track->createNodeKeyFrame(0); // start position
			key->setTranslate(Vector3(0.0,174.0,200.0));
			key = track->createNodeKeyFrame(1);
			key->setTranslate(Vector3(200.0,74.0,0.0));
			key = track->createNodeKeyFrame(2);
			key->setTranslate(Vector3(0.0,74.0,-200.0));
			key = track->createNodeKeyFrame(3);
			key->setTranslate(Vector3(-200,74.0,0.0));
			// Create a new animation state to track this
			AnimationState* mASHead1 = mSceneMgr->createAnimationState("head1AnimationTrack");
			mASHead1->setEnabled(true);

			pEnt = mSceneMgr->createEntity("head1", "ogrehead.mesh");
			sn = mSceneMgr->getRootSceneNode()->createChildSceneNode("snhead1",	Vector3(-200.0, 74.0, 100.0));
			sn->attachObject(pEnt);
			sn->yaw(Ogre::Degree(315-90));

			pEnt = mSceneMgr->createEntity("head2", "ogrehead.mesh");
			sn = mSceneMgr->getRootSceneNode()->createChildSceneNode("snhead2",
				Vector3(0.0, 74.0, -200.0));
			sn->attachObject(pEnt);
			sn->yaw(Ogre::Degree(225+90));

			pEnt = mSceneMgr->createEntity("head3", "ogrehead.mesh");
			sn = mSceneMgr->getRootSceneNode()->createChildSceneNode("snhead3",
				Vector3(-200.0, 74.0, 0.0));
			sn->attachObject(pEnt);
			sn->yaw(Ogre::Degree(135-90));

			// Create the walking list
			mWalkList.push_back(Vector3(0.0f,  174.0f,  200.0f ));
			mWalkList.push_back(Vector3(200.0f,  74.0f, 0.0f));
			mWalkList.push_back(Vector3(0.0f,  74.0f,  -200.0f ));
			mWalkList.push_back(Vector3(-200.0f,  74.0f, 0.0f));

			// Camera.
			mCamera->setPosition(-400.0, 100.0, 150.0);
			mCamera->lookAt(0.0, 20.0, 0.0);
			mCamera->yaw(Degree(-15));
			mCamera->pitch(Degree(20));
		}

		// Basic scene.
		//{
		//	// Floor.
		//	Plane plane(Vector3::UNIT_Y, 0.0);
		//	MeshManager::getSingleton().createPlane(
		//		"FloorPlane", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		//		plane, 200000, 200000, 20, 20, true, 1, 500, 500, Vector3::UNIT_Z);

		//	Entity* entity = mSceneMgr->createEntity("floor", "FloorPlane");
		//	entity->setMaterialName("Examples/RustySteel");
		//	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entity);

		//	// Ogre heads.
		//	entity = mSceneMgr->createEntity("head0", "ogrehead.mesh");
		//	mSceneMgr->getRootSceneNode()->createChildSceneNode(
		//		Vector3(0.0, 14.0, 0.0 - 100.0))->attachObject(entity);

		//	entity = mSceneMgr->createEntity("head1", "ogrehead.mesh");
		//	mSceneMgr->getRootSceneNode()->createChildSceneNode(
		//		Vector3(-50.0, 14.0, -50.0 - 100.0))->attachObject(entity);

		//	entity = mSceneMgr->createEntity("head2", "ogrehead.mesh");
		//	mSceneMgr->getRootSceneNode()->createChildSceneNode(
		//		Vector3(70.0, 14.0, -80.0 - 100.0))->attachObject(entity);

		//	// Camera.
		//	mCamera->setPosition(-104.185, 12.1601, 125.511);
		//	mCamera->lookAt(0.0, 20.0, -35.0);
		//}

		// Depth of Field add-on usage.
		{
			g_dof = new DepthOfField(mCamera->getViewport());

			if (!g_dof->init())
				exit(-1);

			g_dof->setEnabled(true);
		}

		buildDepthDebugWindow();
	}

	// Shows an overlay with the depth texture displayed.
	void buildDepthDebugWindow()
	{
		MaterialPtr mat = MaterialManager::getSingleton().create("GGE_DoF_DepthDebug",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		mat->getTechnique(0)->getPass(0)->createTextureUnitState("GGE_DoF_Depth");

		OverlayContainer *oc = dynamic_cast<OverlayContainer*>(
			OverlayManager::getSingleton().createOverlayElement("Panel", "GGE_DoF_DepthDebugPanel"));
		oc->setLeft(0.0);
		oc->setTop(0.0);
		oc->setWidth(.35);
		oc->setHeight(.35);
		oc->setMaterialName("GGE_DoF_DepthDebug");

		g_depthOverlay = OverlayManager::getSingleton().create("GGE_DoF_DepthDebugOverlay");
		g_depthOverlay->add2D(oc);
		g_depthOverlay->setZOrder(250);
	}
};

#endif // _DepthOfField_demo_h_
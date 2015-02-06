#ifndef _DepthOfField_h_
#define _DepthOfField_h_

#include "OgreCompositorInstance.h"
#include "OgreRenderTargetListener.h"

/*!
* \mainpage GGE Depth of Field Add-On
* \version 1.0
* \date 30 November 2007
*
* \section intro_sec Introduction
*
* This is the documentation for the Depth of Field Add-On for GGE. Click the links
* above to navigate.
*/

/*!
* \brief The Depth of Field effect encapsulation.
*
* This class allows any viewport to utilize a depth of field effect. Instead
* of through traditional 'camera-based' DoF calculations, we instead use a
* 'plane' approach. The camera is considered to be at depth 0. There are then
* 4 planes. From nearest to farest: nearBlurP0, nearBlurP1, farBlurP0, farBlurP1.
* From the camera to nearBlurP0, no blur is applied. Between the nearBlur planes,
* a linear fall-off blur is applied. Between the near blur and far blur planes,
* no blur occurs. From farBlurP0 to farBlurP1, a linearly increasing blur is
* applied. Finally, any depth passed farBlurP1 will be completely blurred.
*
* Usage of the effect is simple:
*
*	1). Allocate a DepthOfField object, passing in a viewport to apply the effect to.
*
*	2). Call init().
*
*	3). Enable the effect with: setEnabled(true).
*
* The accessor functions can be used to adjust many parameters in real-time.
*/
class DepthOfField : public Ogre::CompositorInstance::Listener,
	Ogre::RenderTargetListener,
	Ogre::RenderQueue::RenderableListener
{
public:
	/*!
	* \brief Create the depth of field effect.
	* \param vp The viewport that the effect should be applied to.
	*/
	DepthOfField(Ogre::Viewport *vp);
	virtual ~DepthOfField();

	/*!
	* \brief Initialize all textures, materials, and compositors.
	* \return True if initialization successful, false otherwise.
	*/
	bool init();

	void setNearBlurP0(float val);	//!< Set the nearBlurP0 plane distance.
	void setNearBlurP1(float val);	//!< Set the nearBlurP1 plane distance.
	void setFarBlurP0(float val);	//!< Set the farBlurP0 plane distance.
	void setFarBlurP1(float val);	//!< Set the farBlurP1 plane distance.

	float getNearBlurP0() { return m_nearBlurP0; }	//!< Get the nearBlurP0 plane distance.
	float getNearBlurP1() { return m_nearBlurP1; }	//!< Get the nearBlurP1 plane distance.
	float getFarBlurP0() { return m_farBlurP0; }	//!< Get the farBlurP0 plane distance.
	float getFarBlurP1() { return m_farBlurP1; }	//!< Get the farBlurP1 plane distance.

	/*!
	* \brief Enable/disable the entire Depth of Field effect.
	*/
	void setEnabled(bool state) { m_compositor->setEnabled(state); }
	bool getEnabled() { return m_compositor->getEnabled(); }

	/*!
	* \brief Enable/disable the near Depth of Field effect.
	*/
	void setEnabledNearBlur(bool state);
	bool getEnabledNearBlur() { return m_useNearBlur; }

	/*!
	* \brief Enable/disable the far Depth of Field effect.
	*/
	void setEnabledFarBlur(bool state);
	bool getEnabledFarBlur() { return m_useFarBlur; }

private:
	static const int BLUR_DIVISOR;	//!< Affects the 'bluriness' (downsampling) of the blurred image.

	enum PASS_ID
	{
		BLUR_PASS,
		OUTPUT_PASS
	};

	/*!
	* \brief Creates a compositor in-code.
	*/
	bool createCompositor(const Ogre::String &name);
	void removeCompositor();

	/*!
	* \brief Create the depth render texture.
	*/
	bool createDepthRenderTexture(const Ogre::String &name);
	void removeDepthRenderTexture();

	/*!
	* \brief Implementation of Ogre::CompositorInstance::Listener.
	*
	* Called only 1 time (when the material has first been setup).
	*/
	void notifyMaterialSetup(Ogre::uint32 passId, Ogre::MaterialPtr &material);

	/*!
	* \brief Implementation of Ogre::RenderTargetListener.
	*/
	void preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt);
	void postViewportUpdate(const Ogre::RenderTargetViewportEvent &evt);

	/*!
	* \brief Implementation of Ogre::RenderQueue::RenderableListener.
	*/
	bool renderableQueued(Ogre::Renderable *rend, Ogre::uint8 groupID,
		Ogre::ushort priority, Ogre::Technique **ppTech);

	Ogre::Viewport *m_viewport;

	Ogre::CompositorInstance *m_compositor;

	Ogre::MaterialPtr m_depthMaterial;
	Ogre::RenderTexture *m_depthTarget;
	Ogre::Technique *m_depthTechnique;

	Ogre::MaterialPtr m_depthOfFieldMaterial;

	// The 4 plane distances (P0 is closer to camera than P1).
	float m_nearBlurP0, m_nearBlurP1;
	float m_farBlurP0, m_farBlurP1;

	// Allow us to toggle the near blur and far blur.
	bool m_useNearBlur, m_useFarBlur;
};

#endif // _DepthOfField_h_

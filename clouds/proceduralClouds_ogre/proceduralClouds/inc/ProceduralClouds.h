#ifndef _ProceduralClouds_h_
#define _ProceduralClouds_h_

#include "OgreMaterial.h"
#include "OgreTexture.h"

/*!
* \mainpage GGE ProceduralClouds Add-On
* \version 1.0
* \date 25 November 2007
*
* \section intro_sec Introduction
*
* This is the documentation for the Procedural Clouds Add-On for GGE. Click the links
* above to navigate.
*/

/*!
* \brief The procedural clouds encapsulation.
*
* This class provides the application with a realistic, dynamic, procedurally
* generated cloud material. The application only needs to deal with 3 methods:
* init, update, and getMaterialName.
*
* Usage is simple:
*
*	1). Allocate a ProceduralClouds object.
*
*	2). Call init() with the desired parameters.
*
*	3). Get the generated material with getMaterialName() and apply it to some object.
*
*	4). Call update() every frame.
*
* Parameters can be adjusted in real-time with the other methods, but this is
* optional usage.
*/
class ProceduralClouds
{
public:
	ProceduralClouds();
	virtual ~ProceduralClouds() {}

	/*!
	* \brief Initialize all textures and materials. Create starting octaves.
	* \param updateInterval The cloud formation update interval (in seconds).
	* \param cloudCover 0 = no clouds, 1 = max clouds
	* \param cloudSharpness 0 = 'dull' edges, 1 = 'sharp' edges
	* \param scrollU Cloud scrolling in u-direction.
	* \param scrollV Cloud scrolling in v-direction.
	* \return True if initialization successful, false otherwise.
	*/
	bool init(float updateInterval, float cloudCover, float cloudSharpness, float scrollU, float scrollV);

	/*!
	* \brief Animate the material (cloud motion and formation).
	* \param timeSinceLastFrame ms
	*/
	void update(Ogre::Real timeSinceLastFrame);

	/*!
	* \brief Allows the application to get a handle to the cloud material.
	*
	* TODO: For future development, we will want to get rid of the .material file, and
	* just do everything in code. The material name should be generated in-code,
	* and verified to be unique with some unique ID.
	*
	* \return The name of the material created.
	*/
	const Ogre::String &getMaterialName();

	/*!
	* \brief Affects formation speed. # of seconds (> 0).
	*/
	void setCloudFormationUpdateInterval(float delta);
	float getCloudFormationUpdateInterval() { return m_updateInterval; }

	/*!
	* \brief Cloud scrolling speed in U direction. (> 0).
	*/
	void setCloudScrollU(float delta);
	float getCloudScrollU() { return m_scrollU; }

	/*!
	* \brief Cloud scrolling speed in V direction. (> 0).
	*/
	void setCloudScrollV(float delta);
	float getCloudScrollV() { return m_scrollV; }

	/*!
	* \brief Cloud cover. 0 (no clouds) to 1 (max clouds).
	*/
	void setCloudCover(float delta);
	float getCloudCover() { return m_cloudCover; }

	/*!
	* \brief Cloud 'sharpness'. 0 (fluffy) to 1 (hard edges).
	*/
	void setCloudSharpness(float delta);
	float getCloudSharpness() { return m_cloudSharpness; }

	/*!
	* \brief Allows us to cycle through some of the different cloud effects.
	*/
	void setCloudMode(int delta);
	int getCloudMode() { return m_cloudMode; }

private:
	/*!
	* \brief Creates a manual texture of some width/height, and returns it.
	*/
	Ogre::TexturePtr createTexture(Ogre::String name, int w, int h);

	/*!
	* \brief Generate noise for the given texture, overwriting existing data.
	*
	* If prevTexture is supplied, save out texture to prevTexture before
	* generating new values.
	*/
	void generateNoise(Ogre::TexturePtr texture, Ogre::TexturePtr prevTexture);

	/*!
	* \brief Assign the sky some color.
	*
	* TODO: Sky gradients? Or do this entirely in shader?
	*/
	void generateSkyColor(Ogre::TexturePtr skyTexture);

	/*!
	* \brief LERPs between tex1 and tex2 by t. Write results to dest.
	*/
	void interpolateNoise(float t, Ogre::TexturePtr tex1, Ogre::TexturePtr tex2, Ogre::TexturePtr dest);

	Ogre::MaterialPtr m_cloudMat;

	// The textures.
	Ogre::TexturePtr m_skyColor;	//!< TODO: we could have a current/previous sky to do fading!?
	Ogre::TexturePtr m_tex_Octave1, m_tex_Octave1_cur, m_tex_Octave1_pre;
	Ogre::TexturePtr m_tex_Octave2, m_tex_Octave2_cur, m_tex_Octave2_pre;
	Ogre::TexturePtr m_tex_Octave3, m_tex_Octave3_cur, m_tex_Octave3_pre;
	Ogre::TexturePtr m_tex_Octave4, m_tex_Octave4_cur, m_tex_Octave4_pre;

	float m_updateInterval;		//!< Seconds between octave updates.
	float m_scrollU, m_scrollV;	//!< Scroll the cloud texture along u/v.
	float m_cloudCover;			//!< Cloud cover (density).
	float m_cloudSharpness;		//!< "Sharpness" of cloud outline.

	int m_cloudMode;			//!< Different cloud effects.
	const int NUM_MODES;		//!< Must match the # of cases in the shader.
};

#endif // _ProceduralClouds_h_

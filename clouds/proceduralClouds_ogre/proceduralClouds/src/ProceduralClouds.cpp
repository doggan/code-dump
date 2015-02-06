#include "ProceduralClouds.h"

#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgreTechnique.h"
#include "OgreHardwareBuffer.h"
#include "OgreHardwarePixelBuffer.h"

#include <cassert>

using namespace Ogre;

ProceduralClouds::ProceduralClouds() : NUM_MODES(5)
{

}

bool ProceduralClouds::init(float updateInterval, float cloudCover, float cloudSharpness, float scrollU, float scrollV)
{
	m_updateInterval = updateInterval;
	m_scrollU = scrollU;
	m_scrollV = scrollV;
	m_cloudCover = cloudCover;
	m_cloudSharpness = cloudSharpness;
	m_cloudMode = 0;

	srand(time(NULL));

	// For easy access.
	String name = "";

	// Load the material.
	m_cloudMat = MaterialManager::getSingleton().getByName("procCloudMaterial");
	if (!m_cloudMat.get())
		return false;

	m_cloudMat->setTextureFiltering(TFO_BILINEAR);

	// For alpha.
	m_cloudMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	m_cloudMat->getTechnique(0)->getPass(0)->setSceneBlending(SBF_SOURCE_ALPHA, SBF_DEST_COLOUR);
	m_cloudMat->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

	// GLSL for ogre requires us to explicitly number the variables in the order they are
	//	listed in the shader. Ugh.
	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();

	// Sky color texture.
	name = "tex_skyColor";
	m_skyColor = createTexture(name, 128, 128);
	m_cloudMat->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	params->setNamedConstant(name, 0);

	// Create each octave texture.
	name = "tex_octave1";
	m_tex_Octave1_cur = createTexture(name + "_cur", 64, 64);
	m_tex_Octave1_pre = createTexture(name + "_pre", 64, 64);
	m_tex_Octave1 = createTexture(name, 64, 64);
	TextureUnitState *tus = m_cloudMat->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	tus->setTextureFiltering(TFO_BILINEAR);
	tus->setTextureAddressingMode(TextureUnitState::TAM_WRAP);
	params->setNamedConstant(name, 1);

	name = "tex_octave2";
	m_tex_Octave2_cur = createTexture(name + "_cur", 64, 64);
	m_tex_Octave2_pre = createTexture(name + "_pre", 64, 64);
	m_tex_Octave2 = createTexture(name, 64, 64);
	tus = m_cloudMat->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	tus->setTextureFiltering(TFO_BILINEAR);
	tus->setTextureAddressingMode(TextureUnitState::TAM_WRAP);
	params->setNamedConstant(name, 2);

	name = "tex_octave3";
	m_tex_Octave3_cur = createTexture(name + "_cur", 64, 64);
	m_tex_Octave3_pre = createTexture(name + "_pre", 64, 64);
	m_tex_Octave3 = createTexture(name, 64, 64);
	tus = m_cloudMat->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	tus->setTextureFiltering(TFO_BILINEAR);
	tus->setTextureAddressingMode(TextureUnitState::TAM_WRAP);
	params->setNamedConstant(name, 3);

	name = "tex_octave4";
	m_tex_Octave4_cur = createTexture(name + "_cur", 64, 64);
	m_tex_Octave4_pre = createTexture(name + "_pre", 64, 64);
	m_tex_Octave4 = createTexture(name, 64, 64);
	tus = m_cloudMat->getTechnique(0)->getPass(0)->createTextureUnitState(name);
	tus->setTextureFiltering(TFO_BILINEAR);
	tus->setTextureAddressingMode(TextureUnitState::TAM_WRAP);
	params->setNamedConstant(name, 4);

	// Generate some noise.
	generateSkyColor(m_skyColor);
	generateNoise(m_tex_Octave1_pre, TexturePtr(NULL));
	generateNoise(m_tex_Octave1_cur, TexturePtr(NULL));
	generateNoise(m_tex_Octave2_pre, TexturePtr(NULL));
	generateNoise(m_tex_Octave2_cur, TexturePtr(NULL));
	generateNoise(m_tex_Octave3_pre, TexturePtr(NULL));
	generateNoise(m_tex_Octave3_cur, TexturePtr(NULL));
	generateNoise(m_tex_Octave4_pre, TexturePtr(NULL));
	generateNoise(m_tex_Octave4_cur, TexturePtr(NULL));

	params->setNamedConstant("cloudCover", m_cloudCover);
	params->setNamedConstant("cloudSharpness", m_cloudSharpness);
	params->setNamedConstant("scrollSpeedU", m_scrollU);
	params->setNamedConstant("scrollSpeedV", m_scrollV);
	params->setNamedConstant("cloudMode", m_cloudMode);

	return true;
}

void ProceduralClouds::update(Real timeSinceLastFrame)
{
	static float runningTime = 0.0f;

	if (runningTime > m_updateInterval)
	{
		// Generate some new noise, saving out old noise.
		generateNoise(m_tex_Octave1_cur, m_tex_Octave1_pre);
		generateNoise(m_tex_Octave2_cur, m_tex_Octave2_pre);
		generateNoise(m_tex_Octave3_cur, m_tex_Octave3_pre);
		generateNoise(m_tex_Octave4_cur, m_tex_Octave4_pre);

		runningTime -= m_updateInterval;
	}

	// Noise interpolation (cloud formation animation).
	float t = runningTime / m_updateInterval;
	if (t < 0.0f)	t = 0.0f;
	if (t > 1.0f)	t = 1.0f;

	interpolateNoise(t, m_tex_Octave1_pre, m_tex_Octave1_cur, m_tex_Octave1);
	interpolateNoise(t, m_tex_Octave2_pre, m_tex_Octave2_cur, m_tex_Octave2);
	interpolateNoise(t, m_tex_Octave3_pre, m_tex_Octave3_cur, m_tex_Octave3);
	interpolateNoise(t, m_tex_Octave4_pre, m_tex_Octave4_cur, m_tex_Octave4);

	runningTime += timeSinceLastFrame;
}

TexturePtr ProceduralClouds::createTexture(String name, int w, int h)
{
	return TextureManager::getSingleton().createManual(
		name,
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
		w, h, 0,
		PF_R8G8B8, TU_DYNAMIC_WRITE_ONLY);
}

const String &ProceduralClouds::getMaterialName()
{
	return m_cloudMat->getName();
}

void ProceduralClouds::generateNoise(TexturePtr texture, TexturePtr prevTexture)
{
	HardwarePixelBufferSharedPtr pb_cur = texture->getBuffer();

	// Lock the pixel buffer and get a pixel box
	pb_cur->lock(HardwareBuffer::HBL_DISCARD); // For best performance use HBL_DISCARD!
	const PixelBox *pixelBox_cur = &pb_cur->getCurrentLock();

	uint8 *pDest_cur = static_cast<uint8*>(pixelBox_cur->data);

	HardwarePixelBufferSharedPtr pb_pre;
	const PixelBox *pixelBox_pre;
	uint8 *pDest_pre;

	// If we have a previous texture, repeat above steps for it.
	if (!prevTexture.isNull())
	{
		assert(texture->getWidth() == prevTexture->getWidth() &&
			   texture->getHeight() == prevTexture->getHeight());

		pb_pre = prevTexture->getBuffer();
		pb_pre->lock(HardwareBuffer::HBL_DISCARD);
		pixelBox_pre = &pb_pre->getCurrentLock();
		pDest_pre = static_cast<uint8*>(pixelBox_pre->data);
	}

	// Fill in pixel data.
	for (size_t row = 0; row < texture->getHeight(); ++row)
	{
		for(size_t col = 0; col < texture->getWidth(); ++col)
		{
			// Save out current into prev.
			if (!prevTexture.isNull())
			{
				*pDest_pre++ = *(pDest_cur);
				*pDest_pre++ = *(pDest_cur + 1);
				*pDest_pre++ = *(pDest_cur + 2);
				*pDest_pre++ = *(pDest_cur + 3);
			}

			// Noise generation.
			float r = (float)rand() / ((float)RAND_MAX+1.0f);
			int rInt = (int)(r * 255.0f);
			assert(rInt >= 0 && rInt <= 255);

			*pDest_cur++ = rInt; // B
			*pDest_cur++ = rInt; // G
			*pDest_cur++ = rInt; // R
			*pDest_cur++ = 255; // A
		}
	}

	// Unlock the pixel buffer.
	pb_cur->unlock();

	if (!prevTexture.isNull())
		pb_pre->unlock();
}

void ProceduralClouds::generateSkyColor(Ogre::TexturePtr skyTexture)
{
	HardwarePixelBufferSharedPtr pb = skyTexture->getBuffer();

	// Lock the pixel buffer and get a pixel box
	pb->lock(HardwareBuffer::HBL_DISCARD); // For best performance use HBL_DISCARD!
	const PixelBox *pixelBox = &pb->getCurrentLock();

	uint8 *pDest = static_cast<uint8*>(pixelBox->data);

	// Fill in pixel data.
	for (size_t row = 0; row < skyTexture->getHeight(); ++row)
	{
		for(size_t col = 0; col < skyTexture->getWidth(); ++col)
		{
			*pDest++ = 200; // B
			*pDest++ = 50; // G
			*pDest++ = 50; // R
			*pDest++ = 255; // A
		}
	}

	// Unlock the pixel buffer.
	pb->unlock();
}

void ProceduralClouds::interpolateNoise(float t, Ogre::TexturePtr tex1, Ogre::TexturePtr tex2, Ogre::TexturePtr dest)
{
	assert(tex1->getHeight() == tex2->getHeight() && tex2->getHeight() == dest->getHeight() &&
		   tex1->getWidth() == tex2->getWidth() && tex2->getWidth() == dest->getWidth());

	HardwarePixelBufferSharedPtr pb = dest->getBuffer();
	HardwarePixelBufferSharedPtr pb_pre = tex1->getBuffer();
	HardwarePixelBufferSharedPtr pb_cur = tex2->getBuffer();

	// Lock the pixel buffer and get a pixel box
	pb->lock(HardwareBuffer::HBL_DISCARD); // For best performance use HBL_DISCARD!
	const PixelBox *pixelBox = &pb->getCurrentLock();

	pb_pre->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox *pixelBox_pre = &pb_pre->getCurrentLock();

	pb_cur->lock(HardwareBuffer::HBL_DISCARD);
	const PixelBox *pixelBox_cur = &pb_cur->getCurrentLock();

	uint8 *pDest = static_cast<uint8*>(pixelBox->data);
	uint8 *pDest_pre = static_cast<uint8*>(pixelBox_pre->data);
	uint8 *pDest_cur = static_cast<uint8*>(pixelBox_cur->data);

	for (size_t row = 0; row < dest->getHeight(); ++row)
	{
		for(size_t col = 0; col < dest->getWidth(); ++col)
		{
			// Interpolate.
			*pDest++ = (float)*pDest_pre++ * (1.0f - t) + (float)*pDest_cur++ * t;
			*pDest++ = (float)*pDest_pre++ * (1.0f - t) + (float)*pDest_cur++ * t;
			*pDest++ = (float)*pDest_pre++ * (1.0f - t) + (float)*pDest_cur++ * t;
			*pDest++ = (float)*pDest_pre++ * (1.0f - t) + (float)*pDest_cur++ * t;
		}
	}

	// Unlock the pixel buffer.
	pb->unlock();
	pb_pre->unlock();
	pb_cur->unlock();
}

void ProceduralClouds::setCloudFormationUpdateInterval(float delta)
{
	m_updateInterval += delta;
	if (m_updateInterval < 0.0f)
		m_updateInterval = 0.0f;
}

void ProceduralClouds::setCloudScrollU(float delta)
{
	m_scrollU += delta;
	if (m_scrollU < 0.0f)
		m_scrollU = 0.0f;

	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("scrollSpeedU", m_scrollU);
}

void ProceduralClouds::setCloudScrollV(float delta)
{
	m_scrollV += delta;
	if (m_scrollV < 0.0f)
		m_scrollV = 0.0f;

	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("scrollSpeedV", m_scrollV);
}

void ProceduralClouds::setCloudCover(float delta)
{
	m_cloudCover += delta;
	if (m_cloudCover < 0.0f)
		m_cloudCover = 0.0f;
	else if (m_cloudCover > 1.0f)
		m_cloudCover = 1.0f;

	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("cloudCover", m_cloudCover);
}

void ProceduralClouds::setCloudSharpness(float delta)
{
	m_cloudSharpness += delta;
	if (m_cloudSharpness < 0.0f)
		m_cloudSharpness = 0.0f;
	else if (m_cloudSharpness > 1.0f)
		m_cloudSharpness = 1.0f;

	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("cloudSharpness", m_cloudSharpness);
}

void ProceduralClouds::setCloudMode(int delta)
{
	m_cloudMode += delta;
	if (m_cloudMode >= NUM_MODES)
		m_cloudMode = 0;
	else if (m_cloudMode < 0)
		m_cloudMode = NUM_MODES - 1;

	GpuProgramParametersSharedPtr params = m_cloudMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("cloudMode", m_cloudMode);
}
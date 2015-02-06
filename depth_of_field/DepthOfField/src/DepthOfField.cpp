#include "DepthOfField.h"

#include "OgreViewport.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgreTechnique.h"
#include "OgreCompositor.h"
#include "OgreCompositorManager.h"
#include "OgreCompositionTechnique.h"
#include "OgreCompositionTargetPass.h"
#include "OgreCompositionPass.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreRoot.h"
#include "OgreRenderQueueInvocation.h"

#include <cassert>

using namespace Ogre;

const int DepthOfField::BLUR_DIVISOR = 4;

DepthOfField::DepthOfField(Ogre::Viewport *vp)
: m_viewport(vp)
{

}

DepthOfField::~DepthOfField()
{
	removeDepthRenderTexture();
	removeCompositor();
}

bool DepthOfField::init()
{
	if (!createDepthRenderTexture("GGE_DoF_Depth"))
		return false;

	if (!createCompositor("GGE_DoF_Compositor"))
		return false;

	// Initialize.
	m_nearBlurP0 = 10.0f;
	m_nearBlurP1 = 700.0f;
	m_farBlurP0 = 2300.0f;
	m_farBlurP1 = 3500.0f;
	m_useNearBlur = true;
	m_useFarBlur = true;

	// Initialize depth shader uniforms.
	GpuProgramParametersSharedPtr params = m_depthMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("nearBlurP0", m_nearBlurP0);
	params->setNamedConstant("nearBlurP1", m_nearBlurP1);
	params->setNamedConstant("farBlurP0", m_farBlurP0);
	params->setNamedConstant("farBlurP1", m_farBlurP1);

	return true;
}

bool DepthOfField::createDepthRenderTexture(const Ogre::String &name)
{
	// Create the depth render texture.
	TexturePtr depthTexture = TextureManager::getSingleton().createManual(
		name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D, m_viewport->getActualWidth(), m_viewport->getActualHeight(),
		0, PF_L8, TU_RENDERTARGET);

	m_depthTarget = depthTexture->getBuffer()->getRenderTarget();
	Viewport *depthViewport = m_depthTarget->addViewport(m_viewport->getCamera());

	m_depthTarget->addListener(this);

	// Get the technique to use when rendering the depth render texture
	m_depthMaterial = MaterialManager::getSingleton().getByName(name);
	if (!m_depthMaterial.get())
		return false;

	m_depthMaterial->load(); // Needs to be loaded manually.
	m_depthTechnique = m_depthMaterial->getBestTechnique();

	// Create a custom render queue invocation sequence for the depth render texture.
	RenderQueueInvocationSequence *invocationSequence =
		Root::getSingleton().createRenderQueueInvocationSequence(name);

	// Add a render queue invocation to the sequence, and disable shadows for it
	RenderQueueInvocation *invocation = invocationSequence->add(RENDER_QUEUE_MAIN, "main");
	invocation->setSuppressShadows(true);

	// Set the render queue invocation sequence for the depth render texture viewport.
	depthViewport->setRenderQueueInvocationSequenceName(name);

	return true;
}

void DepthOfField::removeDepthRenderTexture()
{
	m_depthTarget->removeListener(this);
}

bool DepthOfField::createCompositor(const Ogre::String &name)
{
	// Create a compositor.
	CompositorPtr compositor = CompositorManager::getSingleton().create(
		name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	
	if (!compositor.get())
		return false;

	{
		// Create technique.
		CompositionTechnique* technique = compositor->createTechnique();
		{
			// Create texture definition 'scene'.
			CompositionTechnique::TextureDefinition* definition = technique->createTextureDefinition("scene");
			definition->width = 0;
			definition->height = 0;
			definition->format = PF_X8R8G8B8;
		}
		{
			// Create texture definition 'blur'.
			CompositionTechnique::TextureDefinition* definition = technique->createTextureDefinition("blur");
			definition->width = m_viewport->getActualWidth() / BLUR_DIVISOR;
			definition->height = m_viewport->getActualHeight() / BLUR_DIVISOR;
			definition->format = PF_X8R8G8B8;
		}
		{
			// Create target pass 'scene'
			CompositionTargetPass* target = technique->createTargetPass();
			target->setOutputName("scene");
			target->setInputMode(CompositionTargetPass::IM_PREVIOUS);
		}
		{
			// Create target pass 'blur'
			CompositionTargetPass* target = technique->createTargetPass();
			target->setOutputName("blur");
			{
				// Create a composition pass
				CompositionPass* pass = target->createPass();
				pass->setIdentifier(BLUR_PASS);
				pass->setMaterialName("GGE_DoF_Gaussian3x3");
				pass->setInput(0, "scene");
			}
		}
		{
			// Build the output target pass
			CompositionTargetPass* output = technique->getOutputTargetPass();
			{
				// Create a composition pass
				CompositionPass* pass = output->createPass();
				pass->setIdentifier(OUTPUT_PASS);
				pass->setMaterialName("GGE_DoF_DepthOfField");
				pass->setInput(0, "scene");
				//pass->setInput(1, "depth"); // render texture
				pass->setInput(2, "blur");
			}
		}
	}

	// Add the compositor, and store the instance pointer.
	m_compositor = CompositorManager::getSingleton().addCompositor(m_viewport, name, 0);
	if (!m_compositor)
		return false;

	compositor->load(); // Needs to be loaded manually.

	// Enable and add this as a listener.
	CompositorManager::getSingleton().setCompositorEnabled(m_viewport, name, true);
	m_compositor->addListener(this);

	return true;
}

void DepthOfField::removeCompositor()
{
	m_compositor->removeListener(this);
}

void DepthOfField::notifyMaterialSetup(uint32 passId, MaterialPtr &material)
{
	switch (passId)
	{
	case BLUR_PASS:
		{
			float pixelSize[2] = {
				1.0f / (m_viewport->getActualWidth() / BLUR_DIVISOR),
				1.0f / (m_viewport->getActualHeight() / BLUR_DIVISOR)
			};

			// Initialize blur shader uniforms.
			GpuProgramParametersSharedPtr fragParams = material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			fragParams->setNamedConstant("pixelSize", pixelSize, 1, 2);
		}
		break;

	case OUTPUT_PASS:
		{
			// Initialize DoF shader uniforms.
			GpuProgramParametersSharedPtr fragParams = material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
			fragParams->setNamedConstant("nearBlurP0", m_nearBlurP0);
			fragParams->setNamedConstant("nearBlurP1", m_nearBlurP1);
			fragParams->setNamedConstant("farBlurP0", m_farBlurP0);
			fragParams->setNamedConstant("farBlurP1", m_farBlurP1);
			fragParams->setNamedConstant("useNearBlur", (int)m_useNearBlur);
			fragParams->setNamedConstant("useFarBlur", (int)m_useFarBlur);

			// Save a ptr to the material.
			m_depthOfFieldMaterial = material;
		}
		break;
	}
}

void DepthOfField::preViewportUpdate(const RenderTargetViewportEvent &evt)
{
	// Add 'this' as a RenderableListener to replace the technique for all renderables
	RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
	queue->setRenderableListener(this);
}

void DepthOfField::postViewportUpdate(const RenderTargetViewportEvent &evt)
{
	// Reset the RenderableListener.
	RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
	queue->setRenderableListener(0);
}

bool DepthOfField::renderableQueued(Renderable *rend, uint8 groupID, ushort priority, Technique **ppTech)
{
	// Replace the technique of all renderables.
	*ppTech = m_depthTechnique;
	return true;
}

void DepthOfField::setNearBlurP0(float val)
{
	if (val < 0.0f)
		val = .01f;
	else if (val >= m_nearBlurP1)
		val = m_nearBlurP1 - .01f;

	m_nearBlurP0 = val;

	GpuProgramParametersSharedPtr params = m_depthMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("nearBlurP0", m_nearBlurP0);

	params = m_depthOfFieldMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("nearBlurP0", m_nearBlurP0);
}

void DepthOfField::setNearBlurP1(float val)
{
	if (val <= m_nearBlurP0)
		val = m_nearBlurP0 + .01f;
	else if (val >= m_farBlurP0)
		val = m_farBlurP0 - .01f;

	m_nearBlurP1 = val;

	GpuProgramParametersSharedPtr params = m_depthMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("nearBlurP1", m_nearBlurP1);

	params = m_depthOfFieldMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("nearBlurP1", m_nearBlurP1);
}

void DepthOfField::setFarBlurP0(float val)
{
	if (val <= m_nearBlurP1)
		val = m_nearBlurP1 + .01f;
	else if (val >= m_farBlurP1)
		val = m_farBlurP1 - .01f;

	m_farBlurP0 = val;

	GpuProgramParametersSharedPtr params = m_depthMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("farBlurP0", m_farBlurP0);

	params = m_depthOfFieldMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("farBlurP0", m_farBlurP0);
}

void DepthOfField::setFarBlurP1(float val)
{
	if (val <= m_farBlurP0)
		val = m_farBlurP0 + .01f;

	m_farBlurP1 = val;

	GpuProgramParametersSharedPtr params = m_depthMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("farBlurP1", m_farBlurP1);

	params = m_depthOfFieldMaterial->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("farBlurP1", m_farBlurP1);
}

void DepthOfField::setEnabledNearBlur(bool state)
{
	m_useNearBlur = state;

	GpuProgramParametersSharedPtr params = m_depthOfFieldMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("useNearBlur", (int)m_useNearBlur);
}

void DepthOfField::setEnabledFarBlur(bool state)
{
	m_useFarBlur = state;

	GpuProgramParametersSharedPtr params = m_depthOfFieldMaterial->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	params->setNamedConstant("useFarBlur", (int)m_useFarBlur);
}
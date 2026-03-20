#include "rendering-system/renderer/rendering_context.h"
#include <assert.h>




namespace TheEngine::RenderingSystem
{
	RenderingContext::RenderingContext(LowLevelGPUSystem& lowLevelGPUSystem, GPUResourceSystem& gpuResourceSystem):
		m_lowLevelGPUSystem(lowLevelGPUSystem),
		m_gpuResourceSystem(gpuResourceSystem)
	{


	}



	void RenderingContext::addTextureResource(const std::string& name, TextureInfo textureInfo)
	{

		m_textureResources[name] = textureInfo;

	}


	TextureInfo RenderingContext::getTextureResource(const std::string& name)
	{

		auto it = m_textureResources.find(name);

		if (it == m_textureResources.end())
		{
			assert(false && ("Failed to get : " + name + "in RenderingContext").c_str());
		}

		return it->second;


	}

	GPUResourceSystem& RenderingContext::getGPUResourceSystem()
	{
		return m_gpuResourceSystem;
	}



	LowLevelGPUSystem& RenderingContext::getLowLevelGPUSystem()
	{
		return m_lowLevelGPUSystem;
	}

}
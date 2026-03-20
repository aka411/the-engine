#include "engine\engine.h"

namespace TheEngine
{






	Engine::Engine(const EngineConfiguration& engineConfiguration) :
		m_platform(engineConfiguration),
		m_renderingSystem(),
		m_assetSystem(m_renderingSystem.getGPUResourceSystem())
	{

	}






	TheEngine::Platform::Platform& Engine::getPlatform()
	{
		return m_platform;

	}




	TheEngine::RenderingSystem::RenderingSystem& Engine::getRenderingSystem()
	{
		return m_renderingSystem;

	}


	TheEngine::AssetSystem::AssetSystem& Engine::getAssetSystem()
	{

		return m_assetSystem;

	}

}
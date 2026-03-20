#pragma once
#include <rendering-system/renderer/rendering_system.h>
#include <asset-system/asset_system.h>
#include <platform/platform.h>
#include <engine/engine_core_data_types.h>



namespace TheEngine
{


	class Engine
	{

	private:

		//use pimpl?
		TheEngine::Platform::Platform m_platform;
		TheEngine::RenderingSystem::RenderingSystem m_renderingSystem;
		TheEngine::AssetSystem::AssetSystem m_assetSystem;


	public:

		Engine(const EngineConfiguration& engineConfiguration);

		TheEngine::Platform::Platform& getPlatform();

		TheEngine::RenderingSystem::RenderingSystem& getRenderingSystem();
		TheEngine::AssetSystem::AssetSystem& getAssetSystem();



	};



}
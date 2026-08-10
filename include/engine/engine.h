#pragma once

#include <asset-system/asset_system.h>
#include <platform/platform.h>
#include <engine/engine_core_data_types.h>
#include <rendering-system/rendering_system.h>
#include <audio-system/audio_system.h>

#include <utils/event-bus/event_bus.h>

namespace TheEngine
{


	class Engine
	{

	private:


		TheEngine::Platform::Platform m_platform;
		TheEngine::RenderingSystem::RenderingSystem m_renderingSystem;
		TheEngine::AssetSystem::AssetSystem m_assetSystem;

		TheEngine::AudioSystem::AudioSystem m_audioSystem;

		EventBus m_eventBus;

	private:

		void initializeMaterials();//Need to remove later

	private:

		void runInternalSystems();

	public:

		Engine(const EngineConfiguration& engineConfiguration);
		~Engine();


		TheEngine::Platform::Platform& getPlatform();

		TheEngine::RenderingSystem::RenderingSystem& getRenderingSystem();
		TheEngine::AssetSystem::AssetSystem& getAssetSystem();
		TheEngine::AudioSystem::AudioSystem& getAudioSystem();








		//Render system calls






	};



}
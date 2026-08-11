#pragma once

#include <asset-system/asset_system.h>
#include <platform/platform.h>
#include <engine/engine_core_data_types.h>
#include <rendering-system/rendering_system.h>
#include <audio-system/audio_system.h>

#include <utils/event-bus/event_bus.h>

namespace TheEngine
{

	class Application;
	class Engine
	{
	private:

		EventBus m_eventBus{};//Has to been destroyed at last

	private:


		TheEngine::Platform::Platform m_platform;
		TheEngine::RenderingSystem::RenderingSystem m_renderingSystem;
		TheEngine::AssetSystem::AssetSystem m_assetSystem;

		TheEngine::AudioSystem::AudioSystem m_audioSystem;



		EventBusConnection m_eventBusConnection{};

		bool m_engineRunning{ false };

	private:

		void initializeMaterials();//Need to remove later

		void engineShutDown(const EngineEvent& event);
	public:

		Engine(const EngineConfiguration& engineConfiguration);
		~Engine();

		void run(Application& app);

		TheEngine::Platform::Platform& getPlatform();

		TheEngine::RenderingSystem::RenderingSystem& getRenderingSystem();
		TheEngine::AssetSystem::AssetSystem& getAssetSystem();
		TheEngine::AudioSystem::AudioSystem& getAudioSystem();



		bool isEngineRunning() const;




		//Render system calls






	};



}
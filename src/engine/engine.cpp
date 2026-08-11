#include <engine/engine.h>
#include <rendering-system/rhi/i_render_device.h>
#include <engine/application.h>

namespace TheEngine
{



	void Engine::initializeMaterials()
	{
		auto& textureSystem = m_assetSystem.getTextureSystem();


		TheEngine::RenderingSystem::TextureHandle defaultWhite = textureSystem.createDefaultTexture(255, 255, 255, 255); // Albedo, Occlusion, Metallic/Rough
		TheEngine::RenderingSystem::TextureHandle defaultBlack = textureSystem.createDefaultTexture(0, 0, 0, 255);       // Emissive
		TheEngine::RenderingSystem::TextureHandle defaultNormal = textureSystem.createDefaultTexture(128, 128, 255, 255); // Normal Map (0.5, 0.5, 1.0)


		TheEngine::AssetSystem::PBRMRCreateInfo defaultMaterialCreateInfo;

		
		// Pure white base, no emissive
		defaultMaterialCreateInfo.baseColorFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		defaultMaterialCreateInfo.emissiveFactor = glm::vec3(0.0f, 0.0f, 0.0f);

	
		defaultMaterialCreateInfo.metallicFactor = 0.0f;
		defaultMaterialCreateInfo.roughnessFactor = 0.5f;




		// Albedo, Occlusion, and Metal-Rough all use the White 1x1 texture
		defaultMaterialCreateInfo.albedoTextureHandle = defaultWhite;
		defaultMaterialCreateInfo.occlusionTextureHandle = defaultWhite;
		defaultMaterialCreateInfo.metallicRoughnessTextureHandle = defaultWhite;

		// Normal uses the Flat (0.5, 0.5, 1.0) texture
		defaultMaterialCreateInfo.normalTextureHandle = defaultNormal;

	
		defaultMaterialCreateInfo.emissiveTextureHandle = defaultBlack;

	

		 m_assetSystem.
			 getMaterialSystem().createDefaultInternalPBRMaterial(defaultMaterialCreateInfo);

	}

	void Engine::engineShutDown(const EngineEvent& event)
	{
		m_engineRunning = false;
	}


	Engine::Engine(const EngineConfiguration& engineConfiguration) :
		m_platform(engineConfiguration),
		
		m_renderingSystem(std::move(m_platform.getWindowSystem().getRenderDevice()), m_platform.getFileSystem(),m_platform.getWindowSystem().getWindowExtent()),//
		m_assetSystem(m_platform, m_renderingSystem.getGPUResourceSystem()),// currently uses fileSystem
		m_audioSystem(m_platform.getFileSystem())
	{

		initializeMaterials();
	
		m_renderingSystem.registerOnEventBus(m_eventBus);

		m_eventBusConnection = m_eventBus.subscribe(EngineEventType::WINDOW_CLOSE, [this](const EngineEvent& event)
			{
				this->engineShutDown(event);
			});

		WindowExtent windowExtent = m_platform.getWindowSystem().getWindowExtent();
		//m_renderingSystem.(windowExtent);
		EngineEvent event;
		event.engineEventType = EngineEventType::WINDOW_RESIZE;
		event.windowResizeEvent.extend = windowExtent;
		m_eventBus.publish(event);

		m_engineRunning = true;
	}

	void Engine::run(Application& app)
	{
		EngineEvent event;
		while (m_platform.getInputSystem().pollEvent(event))
		{
			m_eventBus.publish(event);
			app.onEvent(event);
		}

	}


	Engine::~Engine()
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

	TheEngine::AudioSystem::AudioSystem& Engine::getAudioSystem()
	{
		return m_audioSystem;
	}


	bool Engine::isEngineRunning() const
	{
		return m_engineRunning;
	}
}
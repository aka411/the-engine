#include <engine\engine.h>
#include <rendering-system/rhi/i_render_device.h>

namespace TheEngine
{



	void Engine::initializeMaterials()
	{
		auto& textureSystem = m_assetSystem.getTextureSystem();


		auto whiteTexInfo = textureSystem.createDefaultTexture(255, 255, 255, 255); // Albedo, Occlusion, Metallic/Rough
		auto blackTexInfo = textureSystem.createDefaultTexture(0, 0, 0, 255);       // Emissive
		auto flatNormInfo = textureSystem.createDefaultTexture(128, 128, 255, 255); // Normal Map (0.5, 0.5, 1.0)


		TheEngine::RenderingSystem::TextureHandle defaultWhite = textureSystem.createNewTexture(whiteTexInfo);
		TheEngine::RenderingSystem::TextureHandle defaultBlack = textureSystem.createNewTexture(blackTexInfo);
		TheEngine::RenderingSystem::TextureHandle defaultNormal = textureSystem.createNewTexture(flatNormInfo);


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


	Engine::Engine(const EngineConfiguration& engineConfiguration) :
		m_platform(engineConfiguration),
		
		m_renderingSystem(std::move(m_platform.getWindowSystem().getRenderDevice()), m_platform.getFileSystem(),m_platform.getWindowSystem().getWindowExtent()),//
		m_assetSystem(m_platform, m_renderingSystem.getGPUResourceSystem()),// currently uses fileSystem
		m_audioSystem(m_platform.getFileSystem())
	{

		initializeMaterials();
	
		WindowExtent windowExtent = m_platform.getWindowSystem().getWindowExtent();
		m_renderingSystem.setWindowExtend(windowExtent);

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



}
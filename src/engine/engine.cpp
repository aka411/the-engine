#include "engine\engine.h"

namespace TheEngine
{



	void Engine::initializeMaterials()
	{
		auto& textureSystem = m_assetSystem.getTextureSystem();


		auto whiteTexInfo = textureSystem.createDefaultTexture(255, 255, 255, 255); // Albedo, Occlusion, Metallic/Rough
		auto blackTexInfo = textureSystem.createDefaultTexture(0, 0, 0, 255);       // Emissive
		auto flatNormInfo = textureSystem.createDefaultTexture(128, 128, 255, 255); // Normal Map (0.5, 0.5, 1.0)


		TheEngine::RenderingSystem::TextureInfo defaultWhite = textureSystem.createNewTexture(whiteTexInfo);
		TheEngine::RenderingSystem::TextureInfo defaultBlack = textureSystem.createNewTexture(blackTexInfo);
		TheEngine::RenderingSystem::TextureInfo defaultNormal = textureSystem.createNewTexture(flatNormInfo);


		TheEngine::AssetSystem::PBRMRCreateInfo defaultMaterialCreateInfo;

		// --- Factors ---
		// Pure white base, no emissive
		defaultMaterialCreateInfo.baseColorFactor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		defaultMaterialCreateInfo.emissiveFactor = glm::vec3(0.0f, 0.0f, 0.0f);

		// 0.0 metallic (dielectric) and 0.5 roughness (balanced) is a safe default
		defaultMaterialCreateInfo.metallicFactor = 0.0f;
		defaultMaterialCreateInfo.roughnessFactor = 0.5f;


		// --- Texture Assignments ---
		// Albedo, Occlusion, and Metal-Rough all use the White 1x1 texture
		defaultMaterialCreateInfo.albedoTextureInfo = defaultWhite;
		defaultMaterialCreateInfo.occlusionTextureInfo = defaultWhite;
		defaultMaterialCreateInfo.metallicRoughnessTextureInfo = defaultWhite;

		// Normal uses the Flat (0.5, 0.5, 1.0) texture
		defaultMaterialCreateInfo.normalTextureInfo = defaultNormal;

		// Emissive uses the Black texture
		defaultMaterialCreateInfo.emissiveTextureInfo = defaultBlack;

		// --- Finalize ---
		 m_assetSystem.
			 getMaterialSystem().createDefaultInternalPBRMaterial(defaultMaterialCreateInfo);

	}


	Engine::Engine(const EngineConfiguration& engineConfiguration) :
		m_platform(engineConfiguration),
		//TODO : NEED MORE THOUGHT HERE
		m_renderingSystem(),//
		m_assetSystem(m_platform, m_renderingSystem.getGPUResourceSystem())// currently uses fileSystem
	{

		initializeMaterials();

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
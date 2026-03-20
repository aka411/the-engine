#include "asset-system/texture_system.h"
#include "rendering-system/low-level-gpu-systems/gpu_texture_manager.h"

namespace TheEngine::AssetSystem
{


	TextureSystem::TextureSystem(TheEngine::RenderingSystem::GPUTextureManager& gpuTextureManager):
		m_gpuTextureManager(gpuTextureManager)
	{



	}


	TheEngine::RenderingSystem::TextureInfo TextureSystem::createNewTexture(const TheEngine::RenderingSystem::TextureCreateInfo& textureCreateInfo)
	{

		return m_gpuTextureManager.createNewTexture(textureCreateInfo);
	}





}
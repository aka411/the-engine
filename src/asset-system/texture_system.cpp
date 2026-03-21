#include "asset-system/texture_system.h"
#include "rendering-system/low-level-gpu-systems/gpu_texture_manager.h"
#include <platform/platform.h>


namespace TheEngine::AssetSystem
{


	TextureSystem::TextureSystem(RenderingSystem::GPUTextureManager& gpuTextureManager, TheEngine::Platform::Platform& platform) :
		m_gpuTextureManager(gpuTextureManager),
		m_imageLoader(platform.getFileSystem())
	{



	}


	TheEngine::RenderingSystem::TextureInfo TextureSystem::createNewTexture(const TheEngine::RenderingSystem::TextureCreateInfo& textureCreateInfo)
	{

		return m_gpuTextureManager.createNewTexture(textureCreateInfo);
	}

	RenderingSystem::TextureInfo TextureSystem::loadTexture(const TheEngine::Platform::Path& path)
	{
		return m_gpuTextureManager.createNewTexture(m_imageLoader.loadTextureFile(path));
	}



}
#pragma once
#include "rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h"
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h>
#include <image_loader.h>






namespace TheEngine::RenderingSystem
{
	class GPUTextureManager;
}

namespace TheEngine::Platform
{
	class Path;
	class Platform;
}

namespace TheEngine::AssetSystem
{




	class TextureSystem
	{

	private :


		RenderingSystem::GPUTextureManager& m_gpuTextureManager;
		ImageLoader m_imageLoader;

	public :

		TextureSystem(RenderingSystem::GPUTextureManager& gpuTextureManager, TheEngine::Platform::Platform& platform);

		RenderingSystem::TextureInfo createNewTexture(const RenderingSystem::TextureCreateInfo& textureCreateInfo);

		//Use Path
		RenderingSystem::TextureInfo loadTexture(const TheEngine::Platform::Path& path);//allow sampler settings also?

	};



}
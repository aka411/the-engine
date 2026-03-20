#pragma once
#include "rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h"







namespace TheEngine::RenderingSystem
{
	class GPUTextureManager;
}


namespace TheEngine::AssetSystem
{




	class TextureSystem
	{

	private :


		RenderingSystem::GPUTextureManager& m_gpuTextureManager;

	public :

		TextureSystem(RenderingSystem::GPUTextureManager& gpuTextureManager);

		RenderingSystem::TextureInfo createNewTexture(const RenderingSystem::TextureCreateInfo& textureCreateInfo);



	};



}
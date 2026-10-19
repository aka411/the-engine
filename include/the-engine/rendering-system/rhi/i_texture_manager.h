#pragma once
#include <vector>
#include <assert.h>

#include <rendering-system/engine_handles.h>
#include <rendering-system/rhi/data-structures/gpu_texture_data_structures.h>

namespace TheEngine::RenderingSystem
{



	class ITextureManager
	{

	protected:



		// A way to track "Free" indices for handles
		std::vector<uint32_t> m_freeIndices;

	public:

		ITextureManager() = default;
	

		virtual ~ITextureManager() = default;


		virtual TextureHandle createTexture(const TextureCreateInfo& info) = 0;
		virtual TextureHandle createTexture(const TextureCreateInfo& info, TheEngine::Memory::MemoryBlock&& initialData) = 0;

		virtual void destroyTexture(const TextureHandle& textureHandle) = 0;


	};


}
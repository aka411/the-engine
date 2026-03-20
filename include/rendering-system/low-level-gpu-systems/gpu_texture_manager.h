#pragma once
#include <unordered_map>
#include "glad/glad.h"
#include "rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h"


namespace TheEngine::RenderingSystem
{

	class GPUSamplerManager;
		

	class GPUTextureManager
	{

	private:

		std::unordered_map<uint64_t, GLuint> m_residentHandleToTextureApiHandle;

		size_t m_totalAllocatedTextureMemory = 0;

		GPUSamplerManager& m_gpuSamplerManager;

	public:

		GPUTextureManager(GPUSamplerManager& gpuSamplerManager);
		~GPUTextureManager();

		TextureInfo createNewTexture(const TextureCreateInfo& textureCreateInfo);
		TextureInfo createRenderTarget(const RenderTargetCreateInfo& bufferTextureCreateInfo);



		void destroyTexture(TextureInfo& textureInfo);

		size_t getTotalAllocatedTextureMemory() const;

	};


}
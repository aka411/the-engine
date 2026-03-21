#pragma once
#include <memory>


namespace TheEngine::Platform
{
	class Platform;
}

namespace TheEngine::RenderingSystem
{


	// Forward declarations
	class GPUBufferTransferManager;

	class GPUBufferManager;
	class GPUTextureManager;
	class GPUShaderManager;
	class GPUSamplerManager;

	class GPUPipelineManager;

	class LowLevelGPUSystem
	{

	private:

		//Owner of all
		std::unique_ptr<GPUBufferManager> m_gpuBufferManager;

		std::unique_ptr<GPUBufferTransferManager> m_gpuBufferTransferManager;//GPU buffer manager dependency , in opengl not needed , in vulkan for creating staging buffers

		std::unique_ptr<GPUSamplerManager> m_gpuSamplerManager;
		std::unique_ptr<GPUTextureManager> m_gpuTextureManager;
		std::unique_ptr<GPUShaderManager> m_gpuShaderManager;

		std::unique_ptr<GPUPipelineManager> m_gpuPipelineManager;


	public:


		LowLevelGPUSystem(TheEngine::Platform::Platform& platform);
		~LowLevelGPUSystem(); 


		// Accessors
		GPUBufferTransferManager& getGPUBufferTransferManager();

		GPUBufferManager& getGPUBufferManager();
		GPUTextureManager& getGPUTextureManager();
		GPUShaderManager& getGPUShaderManager();
		GPUSamplerManager& getGPUSamplerManager();

		GPUPipelineManager& getGPUPipelineManager();


	};


}
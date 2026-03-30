#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>
#include <memory-management/memory_data_types.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{






	//TO DO : add funtionality to transfer data from one gpu buffer to another gpubuffer,
	/*
	struct GPUToGPUBufferTransferRequest
	{
	GPUBufferInfo sourceBuffer;
	GPUBufferInfo targetBuffer;

	size_t dataSize;

	size_t sourceOffset = 0;
	size_t targetOffset = 0;


	}
	*/


	class GPUBufferManager;

	class OpenGLTransferManager : public IGPUTransferManager
	{
		//Owns a staging buffer that is used to transfer data to target GPU buffer.

	private:

		bool m_transferInProgress = false;

		GPUBufferManager& m_gpuBufferManager;


		std::vector<GPUBufferTransferRequest> m_transferQueue;



	public:

		OpenGLTransferManager(IGPUBufferManager& gpuBufferManager);

		virtual ~OpenGLTransferManager() override;



		// Path A: The Linear copy (Buffers)
		virtual void transferToBuffer(GPUBufferTransferRequest&& request) override;

		// Path B: The Tiled/Formatted copy (Images/Textures)
		virtual void transferToImage(GPUImageTransferRequest&& request) override;






	};




}
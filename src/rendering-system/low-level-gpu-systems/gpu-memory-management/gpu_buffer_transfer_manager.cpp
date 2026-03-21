#pragma once
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_buffer_transfer_manager.h>
#include <rendering-system/low-level-gpu-systems/gpu_buffer_manager.h>
#include <assert.h>

namespace TheEngine::RenderingSystem
{


	GPUBufferTransferManager::GPUBufferTransferManager(GPUBufferManager& gpuBufferManager) :

		m_gpuBufferManager(gpuBufferManager)

	{


	}

	GPUBufferTransferManager::~GPUBufferTransferManager() 
	{

	}

	void GPUBufferTransferManager::transferDataToGPUBuffer(GPUBufferTransferRequest&& gpuBufferTransferRequest)
	{

		


		if (gpuBufferTransferRequest.targetBuffer.isMapped)
		{
			assert(gpuBufferTransferRequest.targetBuffer.mappedPtr != nullptr && "GPUBuffer created with nullptr mapped pointer and mislabeled as mapped");

			std::uintptr_t baseAddress = reinterpret_cast<std::uintptr_t>(gpuBufferTransferRequest.targetBuffer.mappedPtr);
			std::uintptr_t offsetAddress = baseAddress + gpuBufferTransferRequest.targetOffset;


			assert(baseAddress + gpuBufferTransferRequest.targetBuffer.size >= offsetAddress + gpuBufferTransferRequest.memoryblock.getSize());

			void* finalDest = reinterpret_cast<void*>(offsetAddress);


			memcpy(finalDest, gpuBufferTransferRequest.memoryblock.getData(), gpuBufferTransferRequest.memoryblock.getSize());
		}
		else
		{
			//for MVP we only use mapped type
			assert(false && " GPUBuffer created with wrong flags");
		}


	}

	bool GPUBufferTransferManager::isTransferInProgress() const
	{
		return m_transferInProgress;
	}

}
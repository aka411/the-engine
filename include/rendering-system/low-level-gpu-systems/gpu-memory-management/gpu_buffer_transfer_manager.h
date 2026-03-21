#pragma once
#include <vector>
#include <memory>
#include <cstddef>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>
#include <memory-management/memory_data_types.h>

namespace TheEngine::RenderingSystem
{

	enum class TransferPriority
	{
		LOW,
		MEDIUM,
		HIGH

	};

	struct GPUBufferTransferRequest
	{
		Memory::MemoryBlock memoryblock;

		const GPUBufferInfo targetBuffer;
		const TransferPriority transferPriority;

		const size_t targetOffset;



		GPUBufferTransferRequest(
			Memory::MemoryBlock memoryblock,
			const GPUBufferInfo& targetBuffer,
			const size_t targetOffset,
			const TransferPriority transferPriority
		)
			:
			memoryblock(std::move(memoryblock)),
			targetBuffer(targetBuffer),
			targetOffset(targetOffset),
			transferPriority(transferPriority)
		{
		}
	};


/*

	struct GPUBufferUrgentTransferRequest
	{
		std::byte* data;//need custom deleter
		const size_t dataSize;
		const GPUBufferInfo targetBuffer;

		const size_t targetOffset;



		GPUBufferTransferRequest
		(
			std::byte* data,
			const size_t dataSize,
			const GPUBufferInfo& targetBuffer,
			const size_t targetOffset
		)
			:
			data(data),
			dataSize(dataSize),
			targetBuffer(targetBuffer),
			targetOffset(targetOffset)

		{
		}
	};

	*/

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

	class GPUBufferTransferManager
	{
		//Owns a staging buffer that is used to transfer data to target GPU buffer.

	private:

		bool m_transferInProgress = false;

		GPUBufferManager& m_gpuBufferManager;


		std::vector<GPUBufferTransferRequest> m_transferQueue;



	public:

		GPUBufferTransferManager(GPUBufferManager& gpuBufferManager);
		~GPUBufferTransferManager();

		//void transferDataToGPUBuffer(std::byte* data, size_t dataSize, GPUBuffer& targetBuffer);
		//void transferDataToGPUBuffer(std::byte* data, size_t dataSize, GPUBuffer& targetBuffer, const TransferPriority transferPriority);
		//void transferDataToGPUBuffer( std::unique_ptr<std::byte[]>&& data, const size_t dataSize, const GPUBuffer& targetBuffer, const TransferPriority transferPriority);
		//void transferDataToGPUBuffer(std::unique_ptr<std::byte[]>&& data, const size_t dataSize, const GPUBuffer& targetBuffer, const size_t targetByteOffset, const TransferPriority transferPriority);

		//TODO : Rename to submitCPUToGPUTransferRquest or something
		void transferDataToGPUBuffer(GPUBufferTransferRequest&& gpuBufferTransferRequest);
		//void transferDataToGPUBufferImmediately(GPUBufferTransferRequest& gpuBufferTransferRequest);

		bool isTransferInProgress() const;



	};




}
#pragma once
#include "memory-management/i_gpu_buffer_allocator.h"
#include <cstdint>



namespace TheEngine::Memory
{

	class GPUBufferCircularAllocator : public IGPUBufferAllocator
	{
	private:

		const GPUBufferInfo m_gpuBufferInfo;
		std::uintptr_t m_currentOffset = 0;

	public:

		GPUBufferCircularAllocator(GPUBufferInfo gpuBufferInfo);


		GPUBufferInfo getGPUBufferInfo() override;


		AllocationInfo allocate(const size_t size) override;

	};

}
#pragma once 
#include "memory-management/memory_system_data_types.h"
#include "memory-management/i_gpu_buffer_allocator.h"



namespace TheEngine::Memory
{

	class GPUBufferPerFrameAllocator : public IGPUBufferAllocator	
	{


	private:


		const GPUBufferInfo m_gpuBufferInfo;
		size_t m_currentOffset = 0;

	public:
		GPUBufferPerFrameAllocator(GPUBufferInfo gpuBufferInfo);


		GPUBufferInfo getGPUBufferInfo() override;

		AllocationInfo allocate(const size_t size) override;

		void reset();

	};


}
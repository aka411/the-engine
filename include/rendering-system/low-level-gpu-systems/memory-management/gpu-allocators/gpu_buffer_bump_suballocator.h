#pragma once 
#include "rendering-system/low-level-gpu-systems/memory-management/gpu-allocators/i_gpu_buffer_suballocator.h"


namespace TheEngine::RenderingSystem
{

	class GPUBufferBumpSubAllocator : public IGPUBufferSubAllocator
	{

	private:

		size_t m_currentOffset = 0;

	public:

		GPUBufferBumpSubAllocator(GPUBufferInfo gpuBufferInfo);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo gpuSubAllocationInfo) override;


	};

}
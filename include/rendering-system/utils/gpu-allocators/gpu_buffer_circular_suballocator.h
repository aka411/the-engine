#pragma once
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <cstdint>



namespace TheEngine::RenderingSystem
{

	class GPUBufferCircularSubAllocator : public IGPUBufferSubAllocator
	{
	private:


		size_t m_currentOffset = 0;

	public:

		GPUBufferCircularSubAllocator(GPUBufferInfo gpuBufferInfo);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo gpuSubAllocationInfo) override;


	};

}
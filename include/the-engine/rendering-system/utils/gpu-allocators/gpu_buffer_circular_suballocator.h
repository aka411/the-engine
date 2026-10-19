#pragma once
#include <cstddef>

#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>




namespace TheEngine::RenderingSystem
{

	class GPUBufferCircularSubAllocator : public IGPUBufferSubAllocator
	{
	private:


		size_t m_currentOffset = 0;

	public:

		GPUBufferCircularSubAllocator( const BufferHandle& bufferHandle, const size_t& bufferSize);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo) override;


	};

}
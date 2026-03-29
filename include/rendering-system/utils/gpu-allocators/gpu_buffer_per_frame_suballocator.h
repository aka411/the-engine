#pragma once 
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/i_gpu_buffer_suballocator.h>



namespace TheEngine::RenderingSystem
{
	//more line linear allocator
	class GPUBufferPerFrameSubAllocator : public IGPUBufferSubAllocator	
	{


	private:

		size_t m_currentOffset = 0;

	public:

		GPUBufferPerFrameSubAllocator(GPUBufferInfo gpuBufferInfo);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo gpuSubAllocationInfo) override;

		//do i need this
		void reset();

	};


}
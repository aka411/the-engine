#pragma once
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>


namespace TheEngine::RenderingSystem
{




	//more like abstract class
	class IGPUBufferSubAllocator
	{
	protected:

		const GPUBufferHandle m_gpuBufferHandle;

	public:

		IGPUBufferSubAllocator(GPUBufferHandle gpuBufferHandle);

		//add message to warn gpu memory leak, cause we still have not added methods to clean up buffers
		virtual ~IGPUBufferSubAllocator() = default;

		GPUBufferHandle getGPUBufferHandle();

		virtual GPUSubAllocationInfo allocate(const size_t size) = 0;
		virtual void deallocate(GPUSubAllocationInfo gpuSubAllocationInfo) = 0;

	};

}
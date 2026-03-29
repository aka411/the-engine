
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/gpu_buffer_bump_suballocator.h>
#include <cassert>


namespace TheEngine::RenderingSystem
{

	GPUBufferBumpSubAllocator::GPUBufferBumpSubAllocator(GPUBufferInfo gpuBufferInfo) :
		IGPUBufferSubAllocator(gpuBufferInfo)
	{




	}




	GPUSubAllocationInfo GPUBufferBumpSubAllocator::allocate(const size_t size)
	{


		if (size + m_currentOffset > m_gpuBufferInfo.size)
		{

			assert(0 && " RUN OUT OF MEMORY IN GPU BUFFER SUB BUMP ALLOCATOR ");
			m_currentOffset = 0; //reset for now
		}

		GPUSubAllocationInfo gpuSubAllocationInfo;

		gpuSubAllocationInfo.offset = m_currentOffset;
		m_currentOffset += size;

		gpuSubAllocationInfo.isAllocationSuccessful = true;

		return gpuSubAllocationInfo;

	}

	void GPUBufferBumpSubAllocator::deallocate(GPUSubAllocationInfo gpuSubAllocationInfo)
	{

		//do nothing

	}



}
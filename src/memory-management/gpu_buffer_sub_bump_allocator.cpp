

#include "memory-management/memory_system_data_types.h"
#include "memory-management/gpu_buffer_sub_bump_allocator.h"

#include <cassert>


namespace TheEngine::Memory
{

	GPUBufferSubBumpAllocator::GPUBufferSubBumpAllocator(GPUBufferInfo gpuBufferInfo) : m_gpuBufferInfo(gpuBufferInfo)
	{




	}




	AllocationInfo GPUBufferSubBumpAllocator::allocate(const size_t size)
	{


		if (size + m_currentOffset > m_gpuBufferInfo.size)
		{

			assert(0 && " RUN OUT OF MEMORY IN GPU BUFFER SUB BUMP ALLOCATOR ");
			m_currentOffset = 0; //reset for now
		}

		AllocationInfo allocationInfo;

		allocationInfo.offset = m_currentOffset;
		m_currentOffset += size;

		return allocationInfo;

	}



	GPUBufferInfo GPUBufferSubBumpAllocator::getGPUBufferInfo()
	{
		return m_gpuBufferInfo;
	}

}
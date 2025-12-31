#include "memory-management/gpu_buffer_circular_allocator.h"

namespace TheEngine::Memory
{

	GPUBufferCircularAllocator::GPUBufferCircularAllocator(GPUBufferInfo gpuBufferInfo):
		m_gpuBufferInfo(gpuBufferInfo)
	{



	}





	GPUBufferInfo GPUBufferCircularAllocator::getGPUBufferInfo()
	{
		return m_gpuBufferInfo;
	}




	AllocationInfo GPUBufferCircularAllocator::allocate(const size_t size)
	{

		if (m_currentOffset + size > m_gpuBufferInfo.size)
		{

			m_currentOffset = 0;
		}

		AllocationInfo allocationInfo;

		allocationInfo.offset = m_currentOffset;
		m_currentOffset += size;

		return allocationInfo;

	}








}
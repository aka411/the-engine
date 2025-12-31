#include "memory-management/gpu_buffer_per_frame_allocator.h"

namespace TheEngine::Memory
{

	GPUBufferPerFrameAllocator::GPUBufferPerFrameAllocator(GPUBufferInfo gpuBufferInfo) :
		m_gpuBufferInfo(gpuBufferInfo)
	{




	}

	GPUBufferInfo GPUBufferPerFrameAllocator::getGPUBufferInfo()
	{
		return m_gpuBufferInfo;
	}




	AllocationInfo GPUBufferPerFrameAllocator::allocate(const size_t size)
	{

		if (size + m_currentOffset > m_gpuBufferInfo.size)
		{

			m_currentOffset = 0;
		}

		AllocationInfo allocationInfo;

		allocationInfo.offset = m_currentOffset;
		m_currentOffset += size;

		return allocationInfo;

	}




	void GPUBufferPerFrameAllocator::reset()
	{
		m_currentOffset = 0;
	}

}
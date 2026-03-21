#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/gpu_buffer_per_frame_suballocator.h>
#include <assert.h>

namespace TheEngine::RenderingSystem
{

	GPUBufferPerFrameSubAllocator::GPUBufferPerFrameSubAllocator(GPUBufferInfo gpuBufferInfo) :
		IGPUBufferSubAllocator(gpuBufferInfo)
	{




	}



	GPUSubAllocationInfo GPUBufferPerFrameSubAllocator::allocate(const size_t size)
	{

		if (size + m_currentOffset > m_gpuBufferInfo.size)
		{

			assert(size < m_gpuBufferInfo.size && "Buffer size than requested Allocation size");
			m_currentOffset = 0;

		}


		GPUSubAllocationInfo allocationInfo;

		allocationInfo.offset = m_currentOffset;
		m_currentOffset += size;

		return allocationInfo;

	}

	void GPUBufferPerFrameSubAllocator::deallocate(GPUSubAllocationInfo gpuSubAllocationInfo)
	{

		//do nothing
	}



	void GPUBufferPerFrameSubAllocator::reset()
	{
		m_currentOffset = 0;
	}

}
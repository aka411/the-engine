#include <rendering-system/utils/gpu-allocators/gpu_buffer_per_frame_suballocator.h>
#include <assert.h>

namespace TheEngine::RenderingSystem
{

	GPUBufferPerFrameSubAllocator::GPUBufferPerFrameSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize) :
		IGPUBufferSubAllocator(bufferHandle, bufferSize)
	{




	}



	GPUSubAllocationInfo GPUBufferPerFrameSubAllocator::allocate(const size_t size)
	{

		if (size + m_currentOffset > m_bufferSize)
		{

			assert(size < m_bufferSize && "Buffer size smaller than requested Allocation size");
			m_currentOffset = 0;

		}


		GPUSubAllocationInfo allocationInfo;

		allocationInfo.offset = m_currentOffset;
		allocationInfo.isAllocationSuccessful = true;
		m_currentOffset += size;

		return allocationInfo;

	}

	void GPUBufferPerFrameSubAllocator::deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo)
	{

		//do nothing
	}



	void GPUBufferPerFrameSubAllocator::reset()
	{
		m_currentOffset = 0;
	}

}
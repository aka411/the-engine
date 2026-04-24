
#include <rendering-system/utils/gpu-allocators/gpu_buffer_bump_suballocator.h>
#include <cassert>


namespace TheEngine::RenderingSystem
{
	void GPUBufferBumpSubAllocator::reset()
	{
		m_currentOffset = 0;
	}



	GPUBufferBumpSubAllocator::GPUBufferBumpSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize) :
		IGPUBufferSubAllocator(bufferHandle, bufferSize)
	{




	}




	GPUSubAllocationInfo GPUBufferBumpSubAllocator::allocate(const size_t size)
	{


		if (size + m_currentOffset > m_bufferSize)
		{

			assert(0 && " RUN OUT OF MEMORY IN GPU BUFFER SUB BUMP ALLOCATOR ");
			m_currentOffset = 0; //reset for now
		}

		GPUSubAllocationInfo gpuSubAllocationInfo;

		gpuSubAllocationInfo.offset = m_currentOffset;
		gpuSubAllocationInfo.size = size;
		m_currentOffset += size;

		gpuSubAllocationInfo.isAllocationSuccessful = true;

		return gpuSubAllocationInfo;

	}



	GPUSubAllocationInfo GPUBufferBumpSubAllocator::allocateAligned(const size_t size, const size_t alignment)
	{
		//assert alignement not zero
		const size_t remainder = m_currentOffset % alignment;
		const size_t padding = (remainder == 0) ? 0 : (alignment - remainder);


		if (size + m_currentOffset + padding > m_bufferSize)
		{

			//assert(0 && " RUN OUT OF MEMORY IN GPU BUFFER SUB BUMP ALLOCATOR ");
			return GPUSubAllocationInfo{ .isAllocationSuccessful = false };
		}



		GPUSubAllocationInfo gpuSubAllocationInfo;

		gpuSubAllocationInfo.offset = m_currentOffset + padding;

		m_currentOffset += size + padding;

		gpuSubAllocationInfo.isAllocationSuccessful = true;

		return gpuSubAllocationInfo;



	}



	void GPUBufferBumpSubAllocator::deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo)
	{

		//do nothing

	}





}
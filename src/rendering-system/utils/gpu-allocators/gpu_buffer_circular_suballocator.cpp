#include <rendering-system/utils/gpu-allocators/gpu_buffer_circular_suballocator.h>
#include <assert.h>

namespace TheEngine::RenderingSystem
{

	GPUBufferCircularSubAllocator::GPUBufferCircularSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize) :
		IGPUBufferSubAllocator(bufferHandle, bufferSize)
	{



	}


	GPUSubAllocationInfo GPUBufferCircularSubAllocator::allocate(const size_t size)
	{

		if (m_currentOffset + size > m_bufferSize)
		{
			assert(size < m_bufferSize && "Tried to allocate memory block larger than availabe pool in GPUBufferCircularSubAllocator");
			m_currentOffset = 0;
		}

		GPUSubAllocationInfo gpuSubAllocationInfo;

		gpuSubAllocationInfo.offset = m_currentOffset;
		m_currentOffset += size;
		gpuSubAllocationInfo.isAllocationSuccessful = true;
		return gpuSubAllocationInfo;


	}


	void GPUBufferCircularSubAllocator::deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo)
	{
		//do nothing
	}






}
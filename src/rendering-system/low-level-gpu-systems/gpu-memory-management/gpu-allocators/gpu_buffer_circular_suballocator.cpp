#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/gpu_buffer_circular_suballocator.h>
#include <assert.h>

namespace TheEngine::RenderingSystem
{

	GPUBufferCircularSubAllocator::GPUBufferCircularSubAllocator(GPUBufferInfo gpuBufferInfo):
		IGPUBufferSubAllocator(gpuBufferInfo)
	{



	}


	GPUSubAllocationInfo GPUBufferCircularSubAllocator::allocate(const size_t size)
	{

		if (m_currentOffset + size > m_gpuBufferInfo.size)
		{
			assert(size < m_gpuBufferInfo.size && "Tried to allocate memory block larger than availabe pool in GPUBufferCircularSubAllocator");
			m_currentOffset = 0;
		}

		GPUSubAllocationInfo gpuSubAllocationInfo;

		gpuSubAllocationInfo.offset = m_currentOffset;
		m_currentOffset += size;
		gpuSubAllocationInfo.isAllocationSuccessful = true;
		return gpuSubAllocationInfo;


	}


	void GPUBufferCircularSubAllocator::deallocate(GPUSubAllocationInfo gpuSubAllocationInfo)
	{
		//do nothing
	}






}
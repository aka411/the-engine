#include <rendering-system/utils/gpu-allocators/gpu_buffer_fixed_size_suballocator.h>
#include <assert.h>


namespace TheEngine::RenderingSystem
{

	GPUSubAllocationInfo GPUBufferFixedSizeSubAllocator::getFreeBlockFromFreeList()
	{

		if (m_freeList.size() != 0)
		{
			GPUSubAllocationInfo gpuSubAllocationInfo = m_freeList.back();
			m_freeList.pop_back();
			m_allocatedList[gpuSubAllocationInfo.offset] = gpuSubAllocationInfo;
			return gpuSubAllocationInfo;
		}
		assert(m_currentOffset < m_bufferSize && "GPUBufferFixedSizeSubAllocator : Ran out of memory");



		return GPUSubAllocationInfo{ .isAllocationSuccessful = false };

	}


	GPUBufferFixedSizeSubAllocator::GPUBufferFixedSizeSubAllocator(const size_t fixedSize, const BufferHandle& bufferHandle, const size_t& bufferSize) :
		IGPUBufferSubAllocator(bufferHandle, bufferSize),
		FIXED_SIZE(fixedSize)
	{


	}


	


	GPUSubAllocationInfo GPUBufferFixedSizeSubAllocator::allocate(const size_t size)
	{

		assert(size == FIXED_SIZE && "Tried to allocate size other than initialized for from GPUBufferFixedSizeSubAllocator");


		if (!m_fullyCarvedUp)
		{
			//Not yet fully curved up
			
			//check if we have space for more carving
			if (m_currentOffset + FIXED_SIZE > m_bufferSize)
			{
				//Nope
	
				m_fullyCarvedUp = true;
				//look for freespace in free list;

				return getFreeBlockFromFreeList();

			}
			else
			{

				GPUSubAllocationInfo gpuSubAllocationInfo;
				gpuSubAllocationInfo.offset = m_currentOffset;
				gpuSubAllocationInfo.size = FIXED_SIZE;
				gpuSubAllocationInfo.isAllocationSuccessful = true;

				m_allocatedList[gpuSubAllocationInfo.offset] = gpuSubAllocationInfo;

				m_currentOffset += FIXED_SIZE;

				return gpuSubAllocationInfo;
			}




		}


		//Fully carved up

		return getFreeBlockFromFreeList();

	}


	void GPUBufferFixedSizeSubAllocator::deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo)
	{

		//validate 
		if (gpuSubAllocationInfo.size != FIXED_SIZE)
		{
			assert(false && "GPUBufferFixedSizeSubAllocator: User trying to mess up bookkeeping by deallocating wrong GPUSubAllocationInfo");
			return;
		}

		auto it = m_allocatedList.find(gpuSubAllocationInfo.offset);
		if (it == m_allocatedList.end())
		{
			assert(false && "GPUBufferFixedSizeSubAllocator: User trying to mess up bookkeeping by deallocating wrong GPUSubAllocationInfo");
			return;
		}
	
	
		m_allocatedList.erase(it->first);

		m_freeList.push_back(gpuSubAllocationInfo);


	}

}

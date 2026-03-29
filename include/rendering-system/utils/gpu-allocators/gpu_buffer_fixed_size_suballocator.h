#pragma once 
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu-allocators/i_gpu_buffer_suballocator.h>
#include <map>
#include <vector>


namespace TheEngine::RenderingSystem
{

	class GPUBufferFixedSizeSubAllocator : public IGPUBufferSubAllocator
	{

	private:

		const size_t FIXED_SIZE;

		//Cut buffer Into fixed size blocks and store as linked list?
		//nah linked list is over kill also extra heap allocation with consider something flat
		//GPUSubAllocationInfo* m_freeList = nullptr;
		//GPUSubAllocationInfo* m_allocatedList = nullptr;

		//ON STACK
		//GPUSubAllocationInfo m_freeList[N];
		//GPUSubAllocationInfo m_allocatedList[N];

		bool m_fullyCarvedUp = false;
		size_t m_currentOffset = 0;

		std::vector<GPUSubAllocationInfo> m_freeList;

		//offset
		std::map<size_t,GPUSubAllocationInfo> m_allocatedList;
		GPUSubAllocationInfo getFreeBlockFromFreeList();
	public:

		GPUBufferFixedSizeSubAllocator(const size_t size, GPUBufferInfo gpuBufferInfo);


		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo gpuSubAllocationInfo) override;


	};


}

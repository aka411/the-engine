#pragma once
#include <rendering-system/utils/gpu-allocators/i_gpu_buffer_suballocator.h>

namespace TheEngine::RenderingSystem
{

	//There are several ways design can go , 
	// 
	// like we can either use a single large buffer and sub allocate from it or we can have multiple buffers and sub allocate from them , 
	// for now we will go with the first approach as it is simpler to implement and manage ,
	// but in future we can consider the second approach if we find that it is more efficient for our use case

	//multiple buffer seems to be right way to go but issue is this
	// buffers is needed by rendering system but if we use
	// multiple buffers there is no way to know which offset belongs to which buffer , 
	// to solve this the rendering system would require to know that data also hence more coupling

	/*  TLSF Header 
	
	
	<---- free list header ----> 

	Linked List Helper methods :-> Put first, Put last, Pop top, Pop bottom.


	
	*/




	class GPUBufferTLSFSubAllocator : public IGPUBufferSubAllocator
	{

	private:
		class TlsfBlockHeader;

		uint32_t m_firstLevelBitmap = 0; // Bitmap for the first level of free blocks
		uint8_t m_secondLevelBitmap[32] = { 0 }; // Bitmap for the second level of free blocks,

		const size_t m_subBinCount = sizeof(m_secondLevelBitmap[0]) * 8;

		TlsfBlockHeader* m_freeList[32][8] = { nullptr }; // Free list for managing free blocks


	public:

		GPUBufferTLSFSubAllocator(const BufferHandle& bufferHandle, const size_t& bufferSize);

		virtual ~GPUBufferTLSFSubAllocator() override;

		virtual GPUSubAllocationInfo allocate(const size_t size) override;
		virtual void deallocate(GPUSubAllocationInfo& gpuSubAllocationInfo) override;
		

	};

}
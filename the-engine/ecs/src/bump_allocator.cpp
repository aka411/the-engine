#include "bump_allocator.h"
#include <cassert>

namespace TheEngine::ECS
{




	AllocatedBlockInfo BumpAllocator::getNewFreeBlock()
	{

		//get new block from os or allocator tree




		return AllocatedBlockInfo();
		
	}

	void BumpAllocator::setNextFreeBlock()
	{
		AllocatedBlockInfo newAllocatedBlockInfo = getNewFreeBlock();
		assert(m_currentAllocatedBlockInfo.ptr != nullptr);
		m_allocatedMemoryBlocks.push_back(newAllocatedBlockInfo);
		m_currentAllocatedBlockInfo = newAllocatedBlockInfo;
		m_currentBaseAddress = reinterpret_cast<std::uintptr_t>(m_currentAllocatedBlockInfo.ptr);
		
	}

	size_t  BumpAllocator::getRequiredPadding(const std::uintptr_t address, const size_t alignment)
	{

		

		size_t requiredPaddingForOffset = alignment - ((address) % alignment);
		if (requiredPaddingForOffset == alignment) requiredPaddingForOffset = 0;


		return requiredPaddingForOffset;
	}



	void* BumpAllocator::allocateAligned(size_t size, size_t alignment)
	{

		//TODO : Implement this

		/*
		if (m_currentAllocatedBlockInfo.sizeOfAllocatedMemory == 0 || m_currentAllocatedBlockInfo.ptr == nullptr)
		{
			setNextFreeBlock();
	
		}

	
		
		const size_t requiredPadding = getRequiredPadding(m_currentBaseAddress + m_currentUsedAmountOffset, alignment);

		if (m_currentUsedAmountOffset + requiredPadding > m_currentAllocatedBlockInfo.sizeOfAllocatedMemory)
		{
			setNextFreeBlock();


			return m_
		}

		
		*/

		//FOR MVP
		return _aligned_malloc(size,alignment);
	}


	void  BumpAllocator::reset()
	{
		//TODO : free allocate blocks or reuse


	}



}
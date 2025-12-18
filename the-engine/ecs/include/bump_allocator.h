#pragma once
#include <vector>
#include <cstdint>


namespace TheEngine::ECS
{
	struct AllocatedBlockInfo
	{
		size_t sizeOfAllocatedMemory = 0;
		void* ptr = nullptr;

	};

	class BumpAllocator
	{

	private:
		std::vector<AllocatedBlockInfo> m_allocatedMemoryBlocks;


		AllocatedBlockInfo m_currentAllocatedBlockInfo;


		std::uintptr_t m_currentBaseAddress = 0;
		size_t m_currentUsedAmountOffset = 0;


		AllocatedBlockInfo getNewFreeBlock(); // affects m_currentAllocatedBlockInfo
		void setNextFreeBlock();
		size_t getRequiredPadding(const std::uintptr_t address, const size_t alignment);

	public:
		void* allocateAligned(size_t size, size_t alignment);
		void reset();

	};
}
#pragma once
#include <cstddef>

namespace TheEngine::Memory
{
	//yeah need care full review here

	class MemoryBlock
	{
	private:
		std::byte* m_data = nullptr;
		size_t m_sizeInBytes = 0;
		//A custom allocator maybe

		//delete copy constructors 
		MemoryBlock(const MemoryBlock&) = delete;
		MemoryBlock& operator=(const MemoryBlock&) = delete;
	public:


		MemoryBlock();
		MemoryBlock(const std::byte* src, const size_t size);
		

		const size_t getSize() const;
		const std::byte* getData() const;
	

		MemoryBlock(MemoryBlock&& memoryBlock) noexcept;
		MemoryBlock& operator=(MemoryBlock&& memoryBlock) noexcept;

		~MemoryBlock();

	};

}
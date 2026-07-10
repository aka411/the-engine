#pragma once
#include <cstddef>

namespace TheEngine::Memory
{


	class MemoryBlock
	{
	private:
		std::byte* m_data = nullptr;
		size_t m_sizeInBytes = 0;



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
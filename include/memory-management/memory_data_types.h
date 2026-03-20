#pragma once
#include <cstddef>
#include <assert.h>
#include <cstring>

namespace TheEngine
{
	//I was thinking of making a centralised memory system from which other systems 
	// can ask for allocators and memory blocks etc

	//we need to redesign the entire flow of data in engine to minimize memory usage etc
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


		MemoryBlock()
		{

		};

		MemoryBlock(const std::byte* src, const size_t size)
		{
			m_data = new std::byte[size];//(size);
			m_sizeInBytes = size;
			assert(m_data != nullptr);
			memcpy(m_data, src, size);
		};


		const size_t getSize() const
		{
			return m_sizeInBytes;
		}

		const std::byte* getData() const
		{
			return m_data;
		}



		MemoryBlock(MemoryBlock&& memoryBlock) noexcept
		{
			this->m_data = memoryBlock.m_data;
			this->m_sizeInBytes = memoryBlock.m_sizeInBytes;

			memoryBlock.m_data = nullptr;
			memoryBlock.m_sizeInBytes = 0;
		};

		MemoryBlock& operator=(MemoryBlock&& memoryBlock) noexcept
		{
			if (this != &memoryBlock)
			{
				this->m_data = memoryBlock.m_data;
				this->m_sizeInBytes = memoryBlock.m_sizeInBytes;

				memoryBlock.m_data = nullptr;
				memoryBlock.m_sizeInBytes = 0;
			}

			return *this;
		};


		~MemoryBlock()
		{
			delete[] m_data;
			m_sizeInBytes = 0;
		};

		//will have custom move etc 
	};


}
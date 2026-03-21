#include <memory-management/memory_block.h>
#include <cstddef>
#include <cstring>
#include <assert.h>


namespace TheEngine::Memory
{

	MemoryBlock::MemoryBlock()
	{

	};

	MemoryBlock::MemoryBlock(const std::byte* src, const size_t size)
	{
		assert(src != nullptr);
		m_data = new std::byte[size];
		m_sizeInBytes = size;

		memcpy(m_data, src, size);
	};


	const size_t MemoryBlock::getSize() const
	{
		return m_sizeInBytes;
	}

	const std::byte* MemoryBlock::getData() const
	{
		return m_data;
	}



	MemoryBlock::MemoryBlock(MemoryBlock&& memoryBlock) noexcept
	{
		this->m_data = memoryBlock.m_data;
		this->m_sizeInBytes = memoryBlock.m_sizeInBytes;

		memoryBlock.m_data = nullptr;
		memoryBlock.m_sizeInBytes = 0;
	};

	MemoryBlock& MemoryBlock::operator=(MemoryBlock&& memoryBlock) noexcept
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


	MemoryBlock::~MemoryBlock()
	{
		//yeah when we use custom allocators need to change this
		delete[] m_data;
		m_sizeInBytes = 0;
	};


}



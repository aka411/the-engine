#include <platform/file-system/file.h>
#include <assert.h>

namespace TheEngine::Platform
{



	File File::createFromPointer(const std::byte* dataPtr, const size_t size)
	{
		File file;
		file.m_buffer.resize(size);
		assert(dataPtr != nullptr && "dataPtr is nullptr");
		memcpy(file.m_buffer.data(), dataPtr, size);
		return file;
	}

	File File::createFromBuffer(std::vector<std::byte>&& buffer)
	{
		File file;
		file.m_buffer = std::move(buffer);

		return file;
	}

	const size_t File::size() const
	{
		return m_buffer.size();
	}

	std::byte* File::data()
	{
		assert(m_buffer.data() != nullptr && "File buffer is null");
		return m_buffer.data();
	}


	std::byte* File::begin()
	{
		return this->data();
	}

	std::byte* File::end()
	{
		return this->data() + m_buffer.size();
	}


	const std::byte* File::data() const
	{
		assert(m_buffer.data() != nullptr && "File buffer is null");
		return m_buffer.data();
	}

	const std::byte* File::begin() const
	{
		return this->data();
	}

	const std::byte* File::end() const
	{
		return this->data() + m_buffer.size();
	}

}
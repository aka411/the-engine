#include <platform/file.h>
#include <assert.h>
#include <utility>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <sys/mman.h>
#else
static_assert(false && "Unsupported Platform");
#endif





namespace TheEngine::Platform
{

	void File::releaseResources() noexcept
	{
		if (m_isMemoryMapped && m_memoryMappedPtr)
		{
			assert(m_fileSize != 0 && "File size is zero");
#ifdef WIN32
			UnmapViewOfFile(m_memoryMappedPtr);
			m_memoryMappedPtr = nullptr;
#elif defined(__linux__)
			assert(m_fd != -1 && "File descriptor missing");
			if (m_fd != -1)
			{
				::munmap(m_memoryMappedPtr, m_fileSize);
				::close(m_fd);
				m_fd = -1;
				m_memoryMappedPtr = nullptr;
			}

#else
			static_assert(false && "Unsupported Platform, Corrupted File destructor");
#endif

		}
	}


	void File::move(File&& other) noexcept
	{
		m_isMemoryMapped = other.m_isMemoryMapped;
		m_memoryMappedPtr = other.m_memoryMappedPtr;
		m_fileSize = other.m_fileSize;
		m_buffer = std::move(other.m_buffer);

#if defined(__linux__)
		m_fd = other.m_fd;

		other.m_fd = -1;
#endif

		other.m_memoryMappedPtr = nullptr;
		other.m_isMemoryMapped = false;
		other.m_fileSize = 0;
	}


	File::File()
	{
	
	}


	File::~File()
	{
		releaseResources();
	}

	File::File(File&& other) noexcept
	{
		move(std::move(other));

	}


	File& File::operator=(File&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}
		this->releaseResources();
		move(std::move(other));

		return *this;
	}


	const std::byte* File::data() const
	{
		if (m_isMemoryMapped)
		{
			return m_memoryMappedPtr;
		}

		return reinterpret_cast<const std::byte*>(m_buffer.data());
	}


	const std::byte& File::operator[](const size_t index) const
	{
		if (index >= m_fileSize)
		{
			assert(false && "Tried to do a out of bound access in File");
		}
		return data()[index];
	}


	const size_t File::size() const
	{
		return m_fileSize;
	}

	const std::byte* File::begin() const
	{
		return data();
	}


	const std::byte* File::end() const
	{
		return data() + m_fileSize;
	}




	const bool File::isValid() const
	{
		if (m_fileSize == 0)
		{
			return false;
		}
		else if (m_isMemoryMapped && m_memoryMappedPtr == nullptr)
		{
			return false;
		}
		else if (!m_isMemoryMapped && m_buffer.size() == 0)
		{
			return false;
		}
#if defined(__linux__)
		if (m_isMemoryMapped && m_fd == -1)
		{
			return false;
		}
#endif

		return true;
	}

}
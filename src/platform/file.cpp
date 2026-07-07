#include <platform/file.h>

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <assert.h>



namespace TheEngine::Platform
{



	File::File()
	{



	}


	File::~File()
	{
		if (m_useMmap && m_mmapPtr)
		{
			UnmapViewOfFile(m_mmapPtr);
			m_mmapPtr = nullptr;
		}
	}

	File::File(File&& other) noexcept
	{
		assert(m_mmapPtr == nullptr && "Tried to move into a File already pointing to another file, might have casue a memory leak");
		m_useMmap = other.m_useMmap;
		m_mmapPtr = other.m_mmapPtr;
		m_fileSize = other.m_fileSize;


		other.m_mmapPtr = nullptr;
		other.m_useMmap = false;
		other.m_fileSize = 0;
	}



	std::byte* File::data() 
	{
		if (m_useMmap)
		{
			return m_mmapPtr;
		}
		//else
		return reinterpret_cast<std::byte*>(m_buffer.data()); 
	}


	size_t File::size() const 
	{ 
		return m_fileSize; 
	}







}
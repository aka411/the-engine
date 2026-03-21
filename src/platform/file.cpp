#include <platform/file.h>

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>



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
		m_useMmap = other.m_useMmap;
		m_mmapPtr = other.m_mmapPtr;
		m_fileSize = other.m_fileSize;


		other.m_mmapPtr = nullptr;
		other.m_useMmap = false;
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
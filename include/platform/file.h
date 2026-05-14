#pragma once
#include <vector>
#include <cstddef>

namespace TheEngine::Platform
{


	enum class FileAccessStrategy 
	{
		AUTO,       
		//STREAM,  
		MEMORY_MAP,  // Force mmap 
		BUFFERED    // Read entire file into RAM at once
	};


	//This is currently not used 
	enum class FileAccessPattern
	{
		READ_ONLY,
		READ_WRITE

	};



	class FileSystem;
	class File
	{
		friend FileSystem;

	protected:

		bool m_useMmap = false;
		std::byte* m_mmapPtr = nullptr;     

		std::vector<std::byte> m_buffer;//Alignment?    
		//MemoryBlock memoryBlock;
		
		size_t m_fileSize = 0;


	public:

		File();
		~File();

		File(File&& other) noexcept;

	    std::byte* data();
		size_t size() const;
	};




}
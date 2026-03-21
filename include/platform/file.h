#pragma once
#include <vector>
#include <cstddef>

namespace TheEngine::Platform
{


	enum class FileAccessStrategy 
	{
		AUTO,       // Let the engine decide based on size
		//STREAM,  // yeah i dont see any use with this,  MEMORY_MAP is way better
		MEMORY_MAP,  // Force mmap (good for large assets/random access)
		BUFFERED    // Read entire file into RAM at once
	};


	//only example , the flags need to be more detailed like 
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

		std::vector<std::byte> m_buffer;    


		size_t m_fileSize = 0;

	public:

		File();
		~File();

		File(File&& other) noexcept;

	    std::byte* data();
		size_t size() const;
	};




}
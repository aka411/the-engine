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

		bool m_isMemoryMapped = false;

		std::byte* m_memoryMappedPtr = nullptr;     
		std::vector<std::byte> m_buffer;   

		size_t m_fileSize = 0;

#if defined(__linux__)
		int   m_fd = -1;
#endif
	
	private:

		void releaseResources() noexcept;
		void move(File&& other) noexcept;

	public:

		File();
		~File();

		File(File&& other) noexcept;
		File& operator=(File&& other) noexcept;


		const std::byte& operator[](const size_t index) const;
		const size_t size() const;

		const std::byte* data() const;

		const std::byte* begin() const;
		const std::byte* end() const;

		const bool isValid() const;
	};




}
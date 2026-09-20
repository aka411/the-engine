#include <platform/file-system/backend/linux_file_system.h>


#include <fcntl.h>    
#include <sys/stat.h> 
#include <unistd.h>   
#include <sys/mman.h> 


#include <cassert>
#include <stdexcept>

namespace TheEngine::Platform
{

	File LinuxFileSystem::mapFile(const Path& path)
	{


		assert(false && "Implement mapFile() in LinuxFileSystem");

		constexpr int INVALID_LINUX_FILE_DESCRIPTOR = -1;

		int fileDescriptor = INVALID_LINUX_FILE_DESCRIPTOR;


		void* mappedPtrLinux = nullptr;
		ssize_t fileSizeLinux = -1;


		fileDescriptor = ::open(path.getPhysicalPath().c_str(), O_RDONLY);
		if (fileDescriptor == INVALID_LINUX_FILE_DESCRIPTOR)
		{
			assert(false && "Unable to open file");
		}


		struct stat file_stat;
		if (::fstat(handle.fileDescriptor, &file_stat) == -1)
		{
			assert(false && "Error: unable to get file size");
		}
		fileSizeLinux = file_stat.st_size;


		if (fileSizeLinux == -1)
		{
			assert(false && "Unable to get file size");
		}

		mappedPtrLinux = ::mmap(nullptr
			, fileSizeLinux
			, PROT_READ
			, MAP_PRIVATE
			, fileDescriptor
			, 0x00
		);



		if (mappedPtrLinux == MAP_FAILED)
		{
			assert(false && "Error: failed to map file to memory");
		}



		File file{};
		::munmap(mappedPtrLinux, fileSizeLinux);

		return file;

	}

	File LinuxFileSystem::open(const Path& path)
	{
		assert(false && "Implement open() in LinuxFileSystem");
		return File{};
	}


	size_t LinuxFileSystem::getFileSize(const Path& path)
	{
		assert(false && "Implement getFileSize() in LinuxFileSystem");
		return 0;
	}




}
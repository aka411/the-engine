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



		constexpr int INVALID_LINUX_FILE_DESCRIPTOR = -1;

		int fileDescriptor = INVALID_LINUX_FILE_DESCRIPTOR;

		void* mappedPtrLinux = nullptr;
		ssize_t fileSizeLinux = -1;


		m_pathBuffer.clear();

		m_pathBuffer.append(m_baseAssetFolderPath);
		m_pathBuffer.append("/");
		m_pathBuffer.append(path.getRelativePath());


		fileDescriptor = ::open(m_pathBuffer.data(), O_RDONLY);
		if (fileDescriptor == INVALID_LINUX_FILE_DESCRIPTOR)
		{
			assert(false && "Unable to open file");
		}


		struct stat file_stat;
		if (::fstat(fileDescriptor, &file_stat) == -1)
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



		File file = File::createFromPointer(reinterpret_cast<std::byte*>(mappedPtrLinux), fileSizeLinux);
		::munmap(mappedPtrLinux, fileSizeLinux);

		return file;

	}

	LinuxFileSystem::LinuxFileSystem(std::string_view baseAssetFolderPath)
	{
		m_baseAssetFolderPath = baseAssetFolderPath;
	}


	File LinuxFileSystem::open(const Path& path)
	{
		//assert(false && "Implement open() in LinuxFileSystem");
		return  mapFile(path);
	}


	size_t LinuxFileSystem::getFileSize(const Path& path)
	{
		assert(false && "Implement getFileSize() in LinuxFileSystem");
		return 0;
	}




}
#include <iostream>
#include <fstream>
#include <platform/file_system.h>
#include <cassert>
#include <engine/engine_core_data_types.h>
#include <platform/path.h>
#include <filesystem>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#elif defined(__linux__)
#include <fcntl.h>    
#include <sys/stat.h> 
#include <unistd.h>   
#include <sys/mman.h> 
#else
static_assert(false && "Unsupported Platform");
#endif


namespace TheEngine::Platform
{


	File FileSystem::mapToMemory(const Path& path)
	{
		File file;

#ifdef WIN32
		//Windows specific code

		struct AutoHandleWin
		{
			HANDLE fileHandle = INVALID_HANDLE_VALUE;
			HANDLE fileMappingHandle = NULL;


			AutoHandleWin(const AutoHandleWin&) = delete;
			AutoHandleWin& operator=(const AutoHandleWin&) = delete;

			AutoHandleWin() = default;


			~AutoHandleWin()
			{
				if (fileHandle != INVALID_HANDLE_VALUE)
				{
					CloseHandle(fileHandle);
				}
				if (fileMappingHandle != NULL)
				{
					CloseHandle(fileMappingHandle);
				}
			}
		};

		AutoHandleWin handles{};

		handles.fileHandle = CreateFile(
			path.getPhysicalPath().c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);


		if (handles.fileHandle == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Unable to open file");
		}


		LARGE_INTEGER fileSizeWindows;
		if (!GetFileSizeEx(handles.fileHandle, &fileSizeWindows))
		{
			throw std::runtime_error("Unable to get file size");
		}

		handles.fileMappingHandle = CreateFileMapping(
			handles.fileHandle,
			nullptr,
			PAGE_READONLY,
			// Passing zeroes for the high and low max-size params here will allow the
			// entire file to be mappable.
			0,
			0,
			nullptr);



		if (handles.fileMappingHandle == NULL)
		{
			throw std::runtime_error("Unable to map file");
		}

		void* mappedPtrWindows = MapViewOfFile(
			handles.fileMappingHandle,
			FILE_MAP_READ,
			0, // Offset high
			0, // Offset low
			// A zero here indicates we want to map the entire range.
			0);



		assert(mappedPtrWindows != nullptr);

		file.m_memoryMappedPtr = static_cast<std::byte*>(mappedPtrWindows);
		file.m_fileSize = static_cast<size_t>(fileSizeWindows.QuadPart);


#elif defined(__linux__)

		constexpr int INVALID_LINUX_FILE_DESCRIPTOR = -1;
		struct AutoHandleLinux
		{
			int fileDescriptor = INVALID_LINUX_FILE_DESCRIPTOR;

			AutoHandleLinux(const AutoHandleLinux&) = delete;
			AutoHandleLinux& operator=(const AutoHandleLinux&) = delete;

			AutoHandleLinux() = default;

			~AutoHandleLinux()
			{
				if (fileDescriptor != INVALID_LINUX_FILE_DESCRIPTOR)
				{
					::close(fileDescriptor);
				}
			}
		};


		AutoHandleLinux handle{};
		void* mappedPtrLinux = nullptr;
		ssize_t fileSizeLinux = -1;


		// Get read-only file descriptor of file 
		handle.fileDescriptor = ::open(path.getPhysicalPath().c_str(), O_RDONLY);
		if (handle.fileDescriptor == INVALID_LINUX_FILE_DESCRIPTOR)
		{
			throw std::runtime_error("Unable to open file");
		}


		struct stat file_stat;
		if (::fstat(handle.fileDescriptor, &file_stat) == -1)
		{
			throw std::runtime_error("Error: unable to get file size");
		}
		fileSizeLinux = file_stat.st_size;


		if (fileSizeLinux == -1)
		{
			throw std::runtime_error("Unable to get file size");
		}

		mappedPtrLinux = ::mmap(nullptr
			, fileSizeLinux
			, PROT_READ
			, MAP_PRIVATE
			, handle.fileDescriptor
			, 0x00
		);



		if (mappedPtrLinux == MAP_FAILED)
		{
			throw std::runtime_error("Error: failed to map file to memory");
		}




		file.m_fileSize = fileSizeLinux;
		file.m_memoryMappedPtr = reinterpret_cast<std::byte*>(mappedPtrLinux);

#else
		static_assert(false && "Unsupported Platform");

#endif


		file.m_isMemoryMapped = true;
		return file;

	}








	FileSystem::FileSystem(const TheEngine::EngineConfiguration& engineConfiguration)
	{
		//little hacky
		for (int i = 0; i < engineConfiguration.mountPaths.size(); ++i)
		{
			Path::s_MountPaths[i] = engineConfiguration.mountPaths[i];
		}

	}


	File FileSystem::open(const Path& path, const FileAccessStrategy strategy)
	{

		//huh might not be portable, rewrite later
		size_t size = std::filesystem::file_size(path.getPhysicalPath());
		//not checking for existence of file

		switch (strategy)
		{

		case FileAccessStrategy::AUTO:
		{
			if (size < 64 * 1024)
			{ // 64KB Threshold

				//Implement later
				//return loadToBuffer(path); // Uses fstream/fread, whole file copied to RAM
				return mapToMemory(path);
			}
			else
			{
				return mapToMemory(path);  // Uses mmap/MapViewOfFile, disk mapped to virtual address page
			}

		}

		case FileAccessStrategy::MEMORY_MAP:
		{
			return mapToMemory(path);
		}

		case FileAccessStrategy::BUFFERED:
		{
			assert(false && " FileAccessStrategy::BUFFERED not implemented yet");
			//loadToBuffer(path);
		}
		default:
		{
			assert(false && "FileAccessStrategy : UNKNOW FileAccessStrategy used");

		}

		}










	}




}
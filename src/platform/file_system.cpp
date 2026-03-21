#include <iostream>
#include <fstream>
#include <platform/file_system.h>
#include <cassert>
#include <engine/engine_core_data_types.h>
#include <platform/path.h>
#include <filesystem>

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>



namespace TheEngine::Platform
{


	File FileSystem::mapToMemory(const Path& path)
	{
		File file;

		//windows specific code

		HANDLE file_handle = CreateFile(
			path.getPhysicalPath().c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);


		LARGE_INTEGER size;
		if (GetFileSizeEx(file_handle, &size)) {
			file.m_fileSize = static_cast<size_t>(size.QuadPart);
		}

		HANDLE file_mapping_handle = CreateFileMapping(
			file_handle,
			nullptr,
			PAGE_READONLY,
			// Passing zeroes for the high and low max-size params here will allow the
			// entire file to be mappable.
			0,
			0,
			nullptr);

		// We can close this now because the file mapping retains an open handle to
		// the underlying file.
		CloseHandle(file_handle);

		char * data = (char *)MapViewOfFile(
			file_mapping_handle,
			FILE_MAP_READ,
			0, // Offset high
			0, // Offset low
			// A zero here indicates we want to map the entire range.
			0);



		// Close the mapping handle
		CloseHandle(file_mapping_handle);


		file.m_useMmap = true;
		file.m_mmapPtr = reinterpret_cast<std::byte*>(data);
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

		case FileAccessStrategy::AUTO :
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
		default :
		{
			assert(false && "FileAccessStrategy : UNKNOW FileAccessStrategy used");

		}

		}

		








	}




}
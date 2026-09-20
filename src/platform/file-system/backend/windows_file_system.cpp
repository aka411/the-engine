#include <platform/file-system/backend/windows_file_system.h>


#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>

#include <stdexcept>
#include <cassert>

namespace TheEngine::Platform
{

	File WindowsFileSystem::mapFile(const Path& path)
	{

		//assert(false && "Implement mapFile() in WindowsFileSystem");
		//m_pathBuffer = path.getRelativePath() + m_baseAssetFolderPath;
		m_pathBuffer.clear();

		m_pathBuffer.append(m_baseAssetFolderPath);
		m_pathBuffer.append(path.getRelativePath());

		HANDLE fileHandle = INVALID_HANDLE_VALUE;
		HANDLE fileMappingHandle = NULL;

		fileHandle = CreateFile(
			m_pathBuffer.data(),
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);


		if (fileHandle == INVALID_HANDLE_VALUE)
		{
			throw std::runtime_error("Unable to open file");
		}


		LARGE_INTEGER fileSizeWindows;
		if (!GetFileSizeEx(fileHandle, &fileSizeWindows))
		{
			throw std::runtime_error("Unable to get file size");
		}

		fileMappingHandle = CreateFileMapping(
			fileHandle,
			nullptr,
			PAGE_READONLY,
			// Passing zeroes for the high and low max-size params here will allow the
			// entire file to be mappable.
			0,
			0,
			nullptr);



		if (fileMappingHandle == NULL)
		{
			throw std::runtime_error("Unable to map file");
		}

		void* mappedPtrWindows = MapViewOfFile(
			fileMappingHandle,
			FILE_MAP_READ,
			0, // Offset high
			0, // Offset low
			// A zero here indicates we want to map the entire range.
			0);



		assert(mappedPtrWindows != nullptr);

		const int64_t fileSize = fileSizeWindows.QuadPart;
		//static_cast<size_t>(fileSize)
		File file = File::createFromPointer(reinterpret_cast<std::byte*>(mappedPtrWindows), fileSize);
		
		UnmapViewOfFile(mappedPtrWindows);

		return file;
	}


	WindowsFileSystem::WindowsFileSystem(std::string_view baseAssetFolderPath)
	{
		m_baseAssetFolderPath = baseAssetFolderPath;
	}


	File WindowsFileSystem::open(const Path& path)
	{

		return mapFile(path);
	}

	size_t WindowsFileSystem::getFileSize(const Path& path)
	{
		assert(false && "Implement getFileSize() in WindowsFileSystem");

		return 0;
	}

}


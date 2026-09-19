#include <platform/file-system/file_system.h>
#include <cassert>
#include <engine/engine_core_data_types.h>
#include <platform/file-system/path.h>


#ifdef PLATFORM_PC
#include <engine_build_config.h>

	#if defined(__linux__)
#include <platform/file-system/backend/linux_file_system.h>

	#elif(WIN32)

#include <platform/file-system/backend/windows_file_system.h>
	#endif

#elif PLATFORM_ANDROID

#include <platform/file-system/backend/android_file_system.h>

#else
	#error("No Platform")
#endif




namespace TheEngine::Platform
{


	FileSystem::FileSystem(const TheEngine::EngineConfiguration& engineConfiguration)
	{


#ifdef PLATFORM_PC
	#if defined(__linux__)
		m_fileSystem = std::make_unique<LinuxFileSystem>(THE_ENGINE_BASE_ASSET_PATH);

	#elif(WIN32)
	
		m_fileSystem = std::make_unique<WindowsFileSystem>(THE_ENGINE_BASE_ASSET_PATH);

	#endif
#elif PLATFORM_ANDROID

		m_fileSystem = std::make_unique<AndroidFileSystem>(engineConfiguration);

#else
	#error("No Platform")
#endif

	}


	File FileSystem::open(const Path& path)
	{
		assert(path.getRelativePath().data()[path.getRelativePath().size()] == '\0');
		return m_fileSystem->open(path);
	}


	size_t FileSystem::getFileSize(const Path& path)
	{
		assert(path.getRelativePath().data()[path.getRelativePath().size()] == '\0');
		return m_fileSystem->getFileSize(path);
	}

}
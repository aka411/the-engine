#pragma once
#include <platform/file-system/file.h>
#include <platform/file-system/i_file_system.h>
#include <platform/file-system/i_stream_reader.h>

#include <memory>

namespace TheEngine
{
	struct EngineConfiguration;
}

namespace TheEngine::Platform
{

	class Path;

	class FileSystem
	{

	private:

		std::unique_ptr<IFileSystem> m_fileSystem;

	public:


		FileSystem(const TheEngine::EngineConfiguration& engineConfiguration);

		File open(const Path& path);

		size_t getFileSize(const Path& path);

		std::unique_ptr<IStreamReader> getFileStream();
	};





}
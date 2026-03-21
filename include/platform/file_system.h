#pragma once
#include <platform/file.h>




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




		File mapToMemory(const Path& path);

	public:


		FileSystem(const TheEngine::EngineConfiguration& engineConfiguration);

		File open(const Path& path, const FileAccessStrategy strategy = FileAccessStrategy::AUTO);

		//we also need a method or object class to stream data and not load all data into RAM at once
	};





}
#pragma once
#include <platform/file-system/file.h>
#include <platform/file-system/path.h>

namespace TheEngine::Platform
{
	class IFileSystem
	{
	private:

	public:

		virtual File open(const Path& path) = 0;
		virtual size_t getFileSize(const Path& path) = 0;
	};


}
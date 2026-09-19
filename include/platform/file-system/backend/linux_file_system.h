#pragma once
#include <platform/file-system/i_file_system.h>

namespace TheEngine::Platform
{
	class LinuxFileSystem final : public IFileSystem
	{

	private:

		std::string m_baseAssetFolderPath{};
		std::string m_pathBuffer{};

		File mapFile(const Path& path);

	public:

		LinuxFileSystem(std::string_view baseAssetFolderPath);
		virtual File open(const Path& path) override;
		virtual size_t getFileSize(const Path& path) override;
	};


}
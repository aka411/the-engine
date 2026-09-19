#pragma once
#include <platform/file-system/i_file_system.h>
#include <engine/engine_core_data_types.h>


struct AAssetManager;

namespace TheEngine::Platform
{
	class AndroidFileSystem final : public IFileSystem
	{
	private:

		AAssetManager* m_androidAssetManager{ nullptr };

		File mapFile(const Path& path);

	public:

		AndroidFileSystem(const EngineConfiguration& engineConfiguration);
	
		virtual File open(const Path& path) override;
		virtual size_t getFileSize(const Path& path) override;

	};

}
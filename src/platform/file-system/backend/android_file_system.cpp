#include <platform/file-system/backend/android_file_system.h>
#include <cassert>
#include <android/asset_manager.h>
#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace TheEngine::Platform
{




	File AndroidFileSystem::mapFile(const Path& path)
	{
		assert(false && "Implement mapFile() in AndroidFileSystem");
		return File{};
	}

	AndroidFileSystem::AndroidFileSystem(const EngineConfiguration& engineConfiguration)
	{
		m_androidAssetManager = engineConfiguration.androidApp->activity->assetManager;
	}


	File AndroidFileSystem::open(const Path& path)
	{


		AAsset* androidAsset = AAssetManager_open(m_androidAssetManager, path.getRelativePath().data(), AASSET_MODE_BUFFER);

		assert(androidAsset != nullptr);
		const void* dataPtr = AAsset_getBuffer(androidAsset);
		const size_t size = AAsset_getLength64(androidAsset);
		File file = File::createFromPointer(static_cast<const std::byte*>(dataPtr), size);
	
		AAsset_close(androidAsset);

		return file;
	}




	size_t AndroidFileSystem::getFileSize(const Path& path)
	{
		AAsset* androidAsset = AAssetManager_open(m_androidAssetManager, path.getRelativePath().data(), AASSET_MODE_BUFFER);

		const size_t size = AAsset_getLength64(androidAsset);
		AAsset_close(androidAsset);

		return size;
	}


}
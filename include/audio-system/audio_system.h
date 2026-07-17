#pragma once
#include <miniaudio/miniaudio.h>
#include <memory>
#include <vector>
#include <deque>
#include <string>
#include <stack>
#include <unordered_map>
#include <mutex>
#include <platform/file.h>


namespace TheEngine::Platform
{
	class Path;
	class FileSystem;
	//class File;
}



namespace TheEngine::AudioSystem
{
	using AudioAssetId = uint64_t;
	
	using SoundHandle = uint64_t;


	/*
	//disable its copy constructor
	struct AudioSourceComponent
	{
		SoundHandle soundHandle;// ->m_internalAudioSources

		float volume = 1.0f;
		bool loop = false;
	};

	*/

	struct AudioSourceComponent
	{
		SoundHandle soundHandle;// ->m_internalAudioSources
		float volume = 1.0f;

		bool loop = false;
		bool isSpatial = true; 
		bool play = false;

		// Range settings
		float minDistance = 1.0f;
		float maxDistance = 50.0f;
	};


	struct AudioListenerComponent
	{

	};


	class AudioSystem
	{

	private:

		struct EngineVFS 
		{
			//Assuming compiler does not add padding to top
			ma_vfs_callbacks cb; // MUST be first 
			AudioSystem* audioSystem = nullptr;
		};

		EngineVFS engineVFS;

		//ma_vfs_callbacks m_ma_vfs_callbacks;


		std::mutex m_vfsMutex;

		static ma_result vfsOpenStatic(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile);
		static ma_result vfsCloseStatic(ma_vfs* pVFS, ma_vfs_file file);
		static ma_result vfsReadStatic(ma_vfs* pVFS, ma_vfs_file file, void* pBufferOut, size_t bytesToRead, size_t* pBytesRead);
		static ma_result vfsSeekStatic(ma_vfs* pVFS, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin);

		static ma_result vfsTellStatic(ma_vfs* pVFS, ma_vfs_file file, ma_int64* pCursor);
		static ma_result vfsInfoStatic(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo);

		ma_engine m_engine{};
		ma_resource_manager m_resourceManager{};

	

	private:

		struct AudioFileAsset
		{
			TheEngine::Platform::File file;
			int referenceCount = 0;
			std::string vfsPath;
		};


		struct AudioFileState
		{
			size_t currentReadIndex = 0;
			AudioFileAsset* audioFileAsset = nullptr;
		};


		
		struct InternalAudioSource
		{
			ma_resource_manager_data_source dataSource;
			ma_sound maSound;

		};



		std::vector<std::unique_ptr<AudioFileState>> m_audioFileStates;

		//path is used as hash
		std::unordered_map<std::string, std::unique_ptr<AudioFileAsset>> m_openAudioFileAssets;

		//AssetId to path
		std::vector <TheEngine::Platform::Path> m_assetIdToAudioAssetPaths;

	
		std::vector< std::unique_ptr<InternalAudioSource>> m_internalAudioSources;



		TheEngine::Platform::FileSystem&  m_fileSystem;

	private:

		AudioSystem(const AudioSystem&) = delete;
		AudioSystem& operator=(const AudioSystem&) = delete;

	public:

		AudioSystem(TheEngine::Platform::FileSystem& fileSystem);

		~AudioSystem();


		AudioAssetId loadAudioAsset(const TheEngine::Platform::Path& path);

		AudioSourceComponent createAudioSourceWithAssetId(AudioAssetId audioAssetId);//TODO : Use reference counting to track how many point to this asset
		void destroyAudioSourceComponent(AudioSourceComponent& audioSourceComponent);
		
		void setSourcePosition(SoundHandle handle, float x, float y, float z);
		void setListenerTransform(float x, float y, float z, float fwdX, float fwdY, float fwdZ);

		void setLooping(SoundHandle handle, bool shouldLoop);
		void playSource(SoundHandle handle);

	};




}
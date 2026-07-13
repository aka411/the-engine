#include <cstring>
#include <math.h>

#include <audio-system/audio_system.h>
#include <platform/file_system.h>
#include <platform/path.h>
#include <cassert>
#include <algorithm>


namespace TheEngine::AudioSystem
{




	ma_result AudioSystem::vfsOpenStatic(ma_vfs* pVFS, const char* pFilePath, ma_uint32 openMode, ma_vfs_file* pFile)
	{
		auto* audioSystem = reinterpret_cast<EngineVFS*>(pVFS)->audioSystem;

		std::lock_guard<std::mutex> lock(audioSystem->m_vfsMutex);

		TheEngine::Platform::Path path = TheEngine::Platform::Path::fromVFSString(pFilePath);
		auto it = audioSystem->m_openAudioFileAssets.find(path.getVFSString());

		if (it == audioSystem->m_openAudioFileAssets.end())
		{


			auto file =TheEngine::Platform::File(audioSystem->m_fileSystem.open(path));

			if (file.data() == nullptr)
			{
				return MA_NO_DATA_AVAILABLE;
			}

			auto audioFileAsset = std::make_unique<AudioFileAsset>(std::move(file),1, path.getVFSString());

			auto audioFileState = std::make_unique<AudioFileState>(0, audioFileAsset.get());


		
			audioSystem->m_openAudioFileAssets.insert({ path.getVFSString(),std::move(audioFileAsset)});

			*pFile = audioFileState.get();

			audioSystem->m_audioFileStates.push_back(std::move(audioFileState));


			return MA_SUCCESS;
		}


		it->second->referenceCount++;


		auto audioFileState = std::make_unique<AudioFileState>(0, it->second.get());

		*pFile = audioFileState.get();

		audioSystem->m_audioFileStates.push_back(std::move(audioFileState));


		return MA_SUCCESS;




	}

	ma_result AudioSystem::vfsCloseStatic(ma_vfs* pVFS, ma_vfs_file file)
	{
		auto* audioSystem = reinterpret_cast<EngineVFS*>(pVFS)->audioSystem;
		auto* statePtr = reinterpret_cast<AudioFileState*>(file);

		std::lock_guard<std::mutex> lock(audioSystem->m_vfsMutex);


		auto it = std::find_if(audioSystem->m_audioFileStates.begin(),
			audioSystem->m_audioFileStates.end(),
			[statePtr](const std::unique_ptr<AudioFileState>& ptr) {
				return ptr.get() == statePtr;
			});

		if (it != audioSystem->m_audioFileStates.end()) {

			

	


			auto& audioFileAsset = it->get()->audioFileAsset;
			audioFileAsset->referenceCount--;
			if (audioFileAsset->referenceCount == 0)
			{
				audioSystem->m_openAudioFileAssets.erase(audioFileAsset->vfsPath);
			}

			audioSystem->m_audioFileStates.erase(it);

			return MA_SUCCESS;
		}

		return MA_INVALID_ARGS;

	}


	ma_result AudioSystem::vfsReadStatic(ma_vfs* pVFS, ma_vfs_file file, void* pBufferOut, size_t bytesToRead, size_t* pBytesRead)
	{

		auto* state = reinterpret_cast<AudioFileState*>(file);
		auto* asset = state->audioFileAsset;

		size_t fileSize = asset->file.size();
		size_t remaining = (state->currentReadIndex < fileSize) ? (fileSize - state->currentReadIndex) : 0;
		size_t actualRead = std::min(bytesToRead, remaining);

		if (actualRead > 0)
		{
			std::memcpy(pBufferOut, (const char*)asset->file.data() + state->currentReadIndex, actualRead);
			state->currentReadIndex += actualRead;
		}
		

		if (pBytesRead) *pBytesRead = actualRead;
		return (actualRead == 0 && bytesToRead > 0) ? MA_AT_END : MA_SUCCESS;



	}

	ma_result AudioSystem::vfsSeekStatic(ma_vfs* pVFS, ma_vfs_file file, ma_int64 offset, ma_seek_origin origin)
	{


		auto* state = reinterpret_cast<AudioFileState*>(file);
		auto* asset = state->audioFileAsset;
		size_t fileSize = asset->file.size();

		ma_int64 newPos = 0;

		switch (origin)
		{
		case ma_seek_origin_start:
			newPos = offset;
			break;

		case ma_seek_origin_current:
			newPos = static_cast<ma_int64>(state->currentReadIndex) + offset;
			break;

		case ma_seek_origin_end:
			newPos = static_cast<ma_int64>(fileSize) + offset;
			break;
		}

		
		if (newPos < 0)
		{
			state->currentReadIndex = 0;
		}
		else if (static_cast<size_t>(newPos) > fileSize)
		{
			state->currentReadIndex = fileSize;
		}
		else
		{
			state->currentReadIndex = static_cast<size_t>(newPos);
		}


		return MA_SUCCESS;


	}


	ma_result AudioSystem::vfsTellStatic(ma_vfs* pVFS, ma_vfs_file file, ma_int64* pCursor)
	{
		auto* state = reinterpret_cast<AudioFileState*>(file);
		if (!pCursor) return MA_INVALID_ARGS;
		*pCursor = static_cast<ma_int64>(state->currentReadIndex);
		return MA_SUCCESS;
	}

	ma_result AudioSystem::vfsInfoStatic(ma_vfs* pVFS, ma_vfs_file file, ma_file_info* pInfo) 
	{
		auto* state = reinterpret_cast<AudioFileState*>(file);
		pInfo->sizeInBytes = state->audioFileAsset->file.size();
		return MA_SUCCESS;
	}









	AudioSystem::AudioSystem(TheEngine::Platform::FileSystem& fileSystem) :
		m_fileSystem(fileSystem)
	{


		
		engineVFS.cb.onOpen = vfsOpenStatic;
		engineVFS.cb.onClose = vfsCloseStatic;
		engineVFS.cb.onRead = vfsReadStatic;
		engineVFS.cb.onSeek = vfsSeekStatic;


		engineVFS.cb.onTell = vfsTellStatic;
		engineVFS.cb.onInfo = vfsInfoStatic;

		engineVFS.audioSystem = this;


		ma_resource_manager_config rmConfig = ma_resource_manager_config_init();
		rmConfig.pVFS = &engineVFS;
		
	
		ma_result result = ma_resource_manager_init(&rmConfig, &m_resourceManager);
		if (result != MA_SUCCESS)
		{
			assert(false && "Failed to initialize miniaudio Resource Manager");
			return;
		}
		ma_engine_config engineConfig = ma_engine_config_init();
		engineConfig.pResourceManager = &m_resourceManager; 


		result = ma_engine_init(&engineConfig, &m_engine);
		if (result != MA_SUCCESS)
		{
			assert(false && "Failed To Initialize AudioBackend");
		}


	}

	AudioSystem::~AudioSystem()
	{

		ma_engine_uninit(&m_engine);
		// ma_device_uninit(&m_device);
	}




	AudioAssetId AudioSystem::loadAudioAsset(const TheEngine::Platform::Path& path)
	{


		AudioAssetId audioAssetId = m_assetIdToAudioAssetPaths.size();

		m_assetIdToAudioAssetPaths.push_back(path);

		return audioAssetId;

	}




	AudioSourceComponent AudioSystem::createAudioSourceWithAssetId(AudioAssetId audioAssetId)//Use reference conting to track how many point to this asset
	{
		//We have another issue what if we store asset id for later use but after vfs calls close we lose it making AudioAssetId invalid 
		
		//MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE -> whole decoded data in ram
		//MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM -> pcm in ram as needed

	
		if (m_assetIdToAudioAssetPaths.size() < audioAssetId)
		{
			assert(false && "Audio Asset not loaded!");
			return {};
		}

	

		auto& path = m_assetIdToAudioAssetPaths[audioAssetId];
		auto internalSource = std::make_unique<InternalAudioSource>();


		// The Resource Manager will check its cache first
		ma_result result = ma_resource_manager_data_source_init(
			&m_resourceManager,
			path.getVFSString().c_str(),
			MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
			nullptr,
			&internalSource->dataSource
		);

		if (result != MA_SUCCESS)
		{
			assert(false && "Failed at method call ma_resource_manager_data_source_init()");
		}

		result = ma_sound_init_from_data_source(
			&m_engine,
			&internalSource->dataSource,
			0, nullptr,
			&internalSource->maSound
		);

		if (result != MA_SUCCESS)
		{
			assert(false && "Failed at method call ma_sound_init_from_data_source()");
		}



		AudioSourceComponent audioSourceComponent;

		audioSourceComponent.soundHandle = m_internalAudioSources.size();

		m_internalAudioSources.push_back(std::move(internalSource));


		//ma_sound_set_spatialization_enabled(&internalSource->maSound, MA_TRUE);
		// Standard inverse distance attenuation (real-world physics)
		//ma_sound_set_attenuation_model(&internalSource->maSound, ma_attenuation_model_inverse);

		// Set the range: sound starts dropping after 1 meter, silent at 100 meters
		//ma_sound_set_min_distance(&internalSource->maSound, 1.0f);
		//ma_sound_set_max_distance(&internalSource->maSound, 100.0f);

		return audioSourceComponent;
	}


	void AudioSystem::destroyAudioSourceComponent(AudioSourceComponent& audioSourceComponent)
	{
		SoundHandle soundHandle = audioSourceComponent.soundHandle;

		std::lock_guard<std::mutex> lock(m_vfsMutex); 
/*
		auto it = std::find_if(m_audioSources.begin(), m_audioSources.end(),
			[&](const std::unique_ptr<AudioSource>& s) {
				return reinterpret_cast<uint64_t>(s.get()) == component.soundHandle.id;
			});

		if (it != m_audioSources.end()) {
			ma_sound_uninit(&(*it)->maSound); // This triggers vfsClose eventually
			m_audioSources.erase(it);
		}
*/



		if (m_internalAudioSources.size() < soundHandle)
		{
			assert(false && "Tried to destroy Invalid AudioSourceComponent");
			return;
		}

		ma_data_source_uninit(&m_internalAudioSources[soundHandle]->dataSource);
		ma_sound_uninit(&m_internalAudioSources[soundHandle]->maSound);
		
		m_internalAudioSources[soundHandle].reset();
		//TODO : Track free SoundHandles also

		return;
	}






	void AudioSystem::setSourcePosition(SoundHandle handle, float x, float y, float z)
	{


	}

	void AudioSystem::setListenerTransform(float x, float y, float z, float fwdX, float fwdY, float fwdZ)
	{


	}

	void AudioSystem::playSource(SoundHandle handle) 
	{
		if (handle < m_internalAudioSources.size() && m_internalAudioSources[handle]) {
			ma_sound_start(&m_internalAudioSources[handle]->maSound);
		}
	}

	void AudioSystem::setLooping(SoundHandle handle, bool shouldLoop) 
	{
		if (handle < m_internalAudioSources.size() && m_internalAudioSources[handle]) {
			ma_sound_set_looping(&m_internalAudioSources[handle]->maSound, shouldLoop ? MA_TRUE : MA_FALSE);
		}
	}


}
#pragma once
#include <memory>
#include "../../resources/include/resource_manager.h"

#include "gpu_resource_manager.h"



namespace TheEngine::Graphics
{

	class GPUResourceStreamer // alternative cool name GPUResourceStreamer
	{
	private:
		IGPURenderDevice& m_renderDevice;
	private:
		TheEngine::Resource::ResourceManager& m_cpuResourceManager;
		GPUResourceManager& m_gpuResourceManager;

	public:

		GPUResourceStreamer(
			TheEngine::Resource::ResourceManager& cpuResourceManager,
			GPUResourceManager& gpuResourceManager,
			IGPURenderDevice& renderDevice);

		bool requestGPUUpload(const TheEngine::Core::ResourceHandle resourceHandle);


	



	};

}
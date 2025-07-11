#include "../include/gpu_resource_streamer.h"


namespace TheEngine::Graphics
{



	GPUResourceStreamer::GPUResourceStreamer(
		TheEngine::Resource::CPUResourceManager& cpuResourceManager,
		GPUResourceManager& gpuResourceManager,
		IGPURenderDevice& renderDevice)
		: m_cpuResourceManager(cpuResourceManager),
		m_gpuResourceManager(gpuResourceManager),
		m_renderDevice(renderDevice),
		m_cpuGpuResourceConvertor(renderDevice)

	{

	}

	bool GPUResourceStreamer::requestGPUUpload(const TheEngine::Core::ResourceHandle resourceHandle)
	{

		//getting resource from cpu resource manager
		TheEngine::Resource::ICPUResource* cpuResource = m_cpuResourceManager.getResource<TheEngine::Resource::ICPUResource>(resourceHandle);




		if (cpuResource == nullptr)
		{
			return false; // Resource not found
		}



		//convert and uplaod to gpu memory
		std::unique_ptr<IGPUResource> igpuResource = m_cpuGpuResourceConvertor.convertToGPUResource(*cpuResource);


		if (igpuResource == nullptr)
		{
			return false;
		}

		//store opaque gpu resource data in gpu resource manager
		m_gpuResourceManager.storeResource(resourceHandle, std::move(igpuResource));

		return true;



	}

}
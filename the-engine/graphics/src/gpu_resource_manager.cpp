
#include "../include/gpu_resource_manager.h"

namespace TheEngine::Graphics
{

	GPUResourceManager::GPUResourceManager(IGPURenderDevice& renderDevice,std::unique_ptr<GPUResourceStreamer>&& gpuResourceStreamer)
		: m_renderDevice(renderDevice), m_GPUResourceStreamer(std::move(gpuResourceStreamer))
	{
	}
	

	void TheEngine::Graphics::GPUResourceManager::storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<IGPUResource>&& resource)
	{
		if (!resource)
		{
			return; // Handle null resource case
		}

		if (m_gpuResourceMap.find(handle) != m_gpuResourceMap.end())
		{
			// Resource already exists, 
			return; // or throw an exception, or replace the existing resource
		}

		m_gpuResourceMap.insert({ handle, std::move(resource) });
	}




	IGPUResource* GPUResourceManager::getResource(const TheEngine::Core::ResourceHandle handle) const
	{

		auto it = m_gpuResourceMap.find(handle);
		if (it != m_gpuResourceMap.end())
		{
			IGPUResource* baseResource = it->second.get();
			if (baseResource == nullptr) return nullptr;//for safety
	
			return  baseResource; // Returns nullptr if the cast fails
		}
		return nullptr;
	}

	void GPUResourceManager::removeResource(const TheEngine::Core::ResourceHandle handle)
	{
		m_gpuResourceMap.erase(handle);
	}
}
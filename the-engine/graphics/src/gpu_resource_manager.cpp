
#include "../include/gpu_resource_manager.h"

template<typename GPUResourceType>
 void TheEngine::Graphics::GPUResourceManager::storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<GPUResourceType>&& resource)
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

 template<typename GPUResourceType>
 GPUResourceType* TheEngine::Graphics::GPUResourceManager::getResource(const TheEngine::Core::ResourceHandle handle)const
 {
	 if (m_gpuResourceMap.find(handle) != m_gpuResourceMap.end())
	 {
		 IGPUResource* baseResource = m_gpuResourceMap[handle].get();
		 GPUResourceType* desiredResource = dynamic_cast<GPUResourceType*>(baseResource);
		 return desiredResource; // Returns nullptr if the cast fails
	 }
	 return nullptr;
 }

 void TheEngine::Graphics::GPUResourceManager::removeResource(const TheEngine::Core::ResourceHandle handle)
 {
	 m_gpuResourceMap.erase(handle);
 }
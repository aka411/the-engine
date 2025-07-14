#include "gpu_resource_system.h"
#include <type_traits>
#include "../../../resources/include/icpu_resource.h"
#include <iostream>

namespace TheEngine::Graphics
{
    GPUResourceSystem::GPUResourceSystem(IGPUResourceManager& m_gpuResourceManager, TheEngine::Resource::ICPUResourceManager& m_cpuResourceManager, IGPUResourceFactory& m_gpuResourceFactory)
		:m_gpuResourceManager(m_gpuResourceManager), m_cpuResourceManager(m_cpuResourceManager), m_gpuResourceFactory(m_gpuResourceFactory)
    {
    }




    IGPUResource* GPUResourceSystem::getGPUResource(const TheEngine::Core::ResourceHandle handle)
    {
        
        IGPUResource* const igpuResource =  m_gpuResourceManager.getResource(handle);

        if (igpuResource == nullptr)
        {
            TheEngine::Resource::ICPUResource* icpuResource =  m_cpuResourceManager.getCPUResource(handle);
     
           if (icpuResource != nullptr)
           {

               std::unique_ptr<IGPUResource> gpuResource = m_gpuResourceFactory.createGPUResource(*icpuResource);
        
			   if (gpuResource != nullptr)
			   {
				   // Store the GPU resource in the manager
	
				   m_gpuResourceManager.storeResource(handle, std::move(gpuResource));
				   // Return the GPU resource
                   return m_gpuResourceManager.getResource(handle);
			   }
			   //log error : Failed to create GPU resource for handle 
			   return nullptr;
           }
           else
           {
			   //log error :  CPU resource not found for handle
			   
			   return nullptr;
           }

        }

        return igpuResource;
    }


}
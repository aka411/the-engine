#include "gpu_resource_system.h"
#include <type_traits>

namespace TheEngine::Graphics
{

    IGPUResource* GPUResourceSystem::getGPUResource(const TheEngine::Core::ResourceHandle handle) 
    {
        
        IGPUResource* const igpuResource =  m_gpuResourceManager.getResource(handle);

        if (igpuResource == nullptr)
        {
           ICPUResource* icpuResource =  m_cpuResourceManager.getCPUResource(handle);
           if (icpuResource != nullptr)
           {

               std::unique_ptr<IGPUResource> gpuResource = m_gpuResourceFactory.createGPUResource(icpuResource);
			   if (gpuResource != nullptr)
			   {
				   m_gpuResourceManager.addResource(handle, std::move(gpuResource));
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
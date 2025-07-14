#include "gpu_resource_factory.h"


namespace TheEngine::Graphics
{


	GPUResourceFactory::GPUResourceFactory(IGPURenderDevice& renderDevice) : m_renderDevice(renderDevice)
	{
		// Initialize the CPU to GPU converter list with available converters.
		m_CPUToGPUConvertorList[ResourceType::MESH] = std::make_unique<MeshCPUToGPUConverter>(m_renderDevice);

		
	}

		std::unique_ptr<IGPUResource> GPUResourceFactory::createGPUResource(const TheEngine::Resource::ICPUResource& cpuResource)
		{
			auto& it = m_CPUToGPUConvertorList.find(cpuResource.getType());
			if (it != m_CPUToGPUConvertorList.end())
			{


				return it->second->convert(cpuResource);
			}

			//log error : No converter found for the given resource type.

			return nullptr;
		}



	};




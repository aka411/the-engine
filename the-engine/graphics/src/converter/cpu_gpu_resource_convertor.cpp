#include "cpu_gpu_resource_convertor.h"

namespace TheEngine::Graphics
{

	CPUGPUResourceConvertor::CPUGPUResourceConvertor( IGPURenderDevice& renderDevice)
		: m_renderDevice(renderDevice)
	{
	}

	std::unique_ptr<IGPUResource> CPUGPUResourceConvertor::convertToGPUResource(const TheEngine::Resource::ICPUResource& cpuResource)
	{

		auto it = m_CPUToGPUConvertorList.find(cpuResource.getType());
		if (it != m_CPUToGPUConvertorList.end())
		{


			return it->second->convert(cpuResource);
		}




		return nullptr;
	}



}

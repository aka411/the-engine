#pragma once
#include "../../igpu_render_device.h"
#include <unordered_map>
#include "icpu_to_gpu_converter.h"
#include "igpu_resource_factory.h"


namespace TheEngine::Graphics

{

	class GPUResourceFactory : IGPUResourceFactory
	{
	private:

		IGPURenderDevice& m_renderDevice;

		std::unordered_map< ResourceType,std::unique_ptr<ICPUToGPUConverter>> m_CPUToGPUConvertorList;
		
	public:

		GPUResourceFactory(IGPURenderDevice& renderDevice);

		std::unique_ptr<IGPUResource> createGPUResource(const TheEngine::Resource::ICPUResource& cpuResource);



	};

}
#pragma once

#include <unordered_map>
#include "../backends/opengl/opengl_render_device.h"
#include "icpu_to_gpu_converter.h"
#include "../../../resources/include/icpu_resource.h"


namespace TheEngine::Graphics

{

	class CPUGPUResourceConvertor
	{
	private:

		IGPURenderDevice& m_renderDevice;

		std::unordered_map< ResourceType,std::unique_ptr<ICPUToGPUConverter>> m_CPUToGPUConvertorList;
		
	public:

		CPUGPUResourceConvertor(IGPURenderDevice& renderDevice);

		std::unique_ptr<IGPUResource> convertToGPUResource(const TheEngine::Resource::ICPUResource& cpuResource);



	};

}
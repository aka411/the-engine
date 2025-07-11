#pragma once
#include <memory>
#include "gpu_resource_manager.h"
#include "../../resources/include/cpu_resource_manager.h"
#include "converter/cpu_gpu_resource_convertor.h"



namespace TheEngine::Graphics
{

	class GPUResourceStreamer 
	{
	
	private:

		TheEngine::Resource::CPUResourceManager& m_cpuResourceManager;

		GPUResourceManager& m_gpuResourceManager;
		IGPURenderDevice& m_renderDevice;


	    CPUGPUResourceConvertor  m_cpuGpuResourceConvertor;

	public:

		GPUResourceStreamer(
			TheEngine::Resource::CPUResourceManager& cpuResourceManager,
			GPUResourceManager& gpuResourceManager,
			IGPURenderDevice& renderDevice);

		bool requestGPUUpload(const TheEngine::Core::ResourceHandle resourceHandle);


	



	};

}
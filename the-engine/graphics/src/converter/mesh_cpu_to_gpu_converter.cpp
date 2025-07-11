#include "mesh_cpu_to_gpu_converter.h"

namespace TheEngine::Graphics
{



	MeshCPUToGPUConverter::MeshCPUToGPUConverter(IGPURenderDevice& renderDevice) : ICPUToGPUConverter(renderDevice)
	{
	}


	std::unique_ptr<IGPUResource> MeshCPUToGPUConverter::convertToGPUResource(const TheEngine::Resource::ICPUResource& icpuResource)
	{
		//use render device here
		//conversion from cpumesh to gpumesh


		return ;
	}

}
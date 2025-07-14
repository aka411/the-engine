#pragma once
#include "icpu_to_gpu_converter.h"




namespace TheEngine::Graphics
{

	class MeshCPUToGPUConverter : public ICPUToGPUConverter
	{
	private:

		IGPURenderDevice& m_renderDevice;

	public:
		MeshCPUToGPUConverter(IGPURenderDevice& renderDevice);

		std::unique_ptr<IGPUResource> convertToGPUResource(const TheEngine::Resource::ICPUResource& icpuResource) override;
	};
}
#pragma once
#include "../../../resources/include/icpu_resource.h"
#include "../igpu_resource.h"
#include <memory>

#include "../../igpu_resource.h"
#include "../../igpu_render_device.h"

namespace TheEngine::Graphics
{

	class ICPUToGPUConverter 
	{


	private:
		IGPURenderDevice& m_renderDevice;
	protected:

		ICPUToGPUConverter(IGPURenderDevice& renderDevice) :
			m_renderDevice(renderDevice)
		{
		}

	public:
		virtual ~ICPUToGPUConverter() = default;
		// Converts a CPU resource to a GPU resource
		
		virtual std::unique_ptr<IGPUResource>  convertToGPUResource(const TheEngine::Resource::ICPUResource& icpuResource) = 0;
	};
}
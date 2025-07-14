#pragma once
#include <memory>
#include "../../igpu_resource.h"
#include "../../../resources/include/icpu_resource.h"

namespace TheEngine::Graphics
{
	class IGPUResourceFactory
	{
	public:
		virtual ~IGPUResourceFactory() = default;

		
		virtual std::unique_ptr<IGPUResource> createGPUResource(const TheEngine::Resource::ICPUResource& cpuResource) = 0;
	};

}
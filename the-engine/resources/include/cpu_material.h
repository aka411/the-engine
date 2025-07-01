#pragma once
#include "cpu_resource.h"
#include <vector>
#include "../../graphics/include/core/pipeline_resource_layout.h"
#include "../../core/include/types.h"

namespace TheEngine::Resource
{
	class CPUMaterial : public CPUResource
	{
	public:
		std::vector<TheEngine::Core::ResourceHandle> materialResourceHandles;
		TheEngine::Graphics::PipelineResourceLayout pipelineResourceLayout;

		//bool ismaterialConstants;
		//TheEngine::Core::ResourceHandle materialConstantsBufferHandle;
	public:
		CPUResourceType getType() const override { return CPUResourceType::MATERIAL; }
	};
}
#pragma once
#include "../igpu_resource.h"
#include "../../../core/include/types.h"


namespace TheEngine::Graphics
{
	class IGPUResourceSystem
	{

	public:
		virtual ~IGPUResourceSystem() = default;
		virtual IGPUResource* getGPUResource(const TheEngine::Core::ResourceHandle handle)  = 0;

	};
}
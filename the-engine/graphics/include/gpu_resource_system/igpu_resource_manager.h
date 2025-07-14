#pragma once
#include <memory>
#include "../../../core/include/types.h"

namespace TheEngine::Graphics
{

	class IGPUResourceManager
	{
	public:
		virtual ~IGPUResourceManager() = default;
		virtual IGPUResource* getResource(const TheEngine::Core::ResourceHandle handle) const = 0;
		virtual void storeResource(const TheEngine::Core::ResourceHandle handle, std::unique_ptr<IGPUResource>&& resource) = 0;
	};


}
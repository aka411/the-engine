#pragma once
#include "icpu_resource.h"
namespace TheEngine::Resource
{

	class ICPUResourceManager
	{
	public:

		virtual ICPUResource* getCPUResource(const TheEngine::Core::ResourceHandle handle) const = 0;



	};

}
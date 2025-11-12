#pragma once
#include <functional>
#include <typeindex>
#include "common_data_types.h"

namespace TheEngine::ECS
{
	







	class IComponentRegistry
	{

	private:

	public:

		virtual ComponentTypeInfo* getComponentTypeInfo(ComponentId componentId) const = 0;

	
	};

}
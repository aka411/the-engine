#include "component_registry.h"
#include <stdexcept>
#include <typeindex>

namespace TheEngine::ECS
{
	

	ComponentTypeinfo* ComponentRegistry::getComponentTypeinfo(ComponentId componentId) const
	{
		auto it = m_IdToInfo.find(componentId);
		if (it != m_IdToInfo.end())
		{
			return it->second;
		}
		// Handle the case where the component ID is not found
		throw std::runtime_error("Component ID not found");
		//return nullptr;
	}

	ComponentId ComponentRegistry::getComponentId(std::type_index typeIndex) const
	{
		{
			auto it = m_typeToId.find(typeIndex);
			if (it != m_typeToId.end())
			{
				return it->second;
			}
			return -1; // negative value to signal inavild id 
		}
	}

	void ComponentRegistry::removeComponent(ComponentId componentId)
	{
		auto it = m_IdToInfo.find(componentId);
		// Check if the component ID exists in the registry
		if (it != m_IdToInfo.end())
		{
			m_typeToId.erase(it->second.typeId);
			m_IdToInfo.erase(it);
			
		}
		else
		{
			throw std::runtime_error("Component ID not found");
		}

	}


}
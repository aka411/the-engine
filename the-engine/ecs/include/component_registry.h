#pragma once
#include "i_component_registry.h"

namespace TheEngine::ECS
{






	class ComponentRegistry : public IComponentRegistry
	{

	private:

		// Internal maps to store registration data
		ComponentId m_nextComponentId = 0;

		std::unordered_map<std::type_index, ComponentId> m_typeIndexToComponentIdMap;

		std::unordered_map<ComponentId, std::unique_ptr<ComponentTypeInfo>> m_componentIdToTypeInfoMap;//use unique pointer?


	public:

		
		ComponentRegistry() = default;
		~ComponentRegistry() = default;

		template<typename DataType>
	    inline ComponentId RegisterComponent(DataType component);

		virtual ComponentTypeInfo* getComponentTypeinfo(ComponentId componentId) const override;


	};



	template<typename DataType>
    ComponentId inline ComponentRegistry::RegisterComponent(DataType component)
	{
		//TODO : IMPLEMENT THIS
		return ComponentId();
	}





}
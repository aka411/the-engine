#pragma once
#include "i_component_registry.h"
#include <memory>
#include "common_data_types.h"
#include <cassert>
#include <stdexcept>

namespace TheEngine::ECS
{


	class ComponentRegistry : public IComponentRegistry
	{

	private:

		// Internal maps to store registration data
		ComponentId m_nextComponentId = 0;

		std::unordered_map<std::type_index, ComponentId> m_typeIndexToComponentIdMap;

		std::unordered_map<ComponentId, std::unique_ptr<ComponentTypeInfo>> m_componentIdToTypeInfoMap;


	public:


		ComponentRegistry() = default;
		~ComponentRegistry() = default;

		template<typename DataType>
		inline ComponentId registerComponent();

		virtual ComponentTypeInfo* getComponentTypeInfo(ComponentId componentId) const override;

		template<typename ComponentType>
		ComponentId getComponentIdFromComponent() const;

	};



	template<typename DataType>
	ComponentId inline ComponentRegistry::registerComponent()
	{



		// 1. Get the canonical type index
		using ComponentType = std::decay_t<DataType>;//to reomove const ,pointer type etc
		std::type_index typeIndex = std::type_index(typeid(ComponentType));

		// 2. Check if already registered
		auto it = m_typeIndexToComponentIdMap.find(typeIndex);
		if (it != m_typeIndexToComponentIdMap.end())
		{
			// Already registered, return the existing ID
			return it->second;
		}
		else
		{
			//TODO : need a add a list of free ids
			m_typeIndexToComponentIdMap[typeIndex] = m_nextComponentId;
			

			//--Create TypeInfo ---//

			std::unique_ptr<ComponentTypeInfo> componentTypeInfo = std::make_unique<ComponentTypeInfo>();

			componentTypeInfo->componentId = m_typeIndexToComponentIdMap[typeIndex];
			componentTypeInfo->size = sizeof(ComponentType);
			componentTypeInfo->alignment = alignof(ComponentType);

			componentTypeInfo->constructor = [](void* ptr)
			{
				new (ptr) ComponentType();

			};

			componentTypeInfo->destructor = [](void* ptr)
			{
				ComponentType* component = reinterpret_cast<ComponentType*>(ptr);
				component->~ComponentType();
			};

			componentTypeInfo->copyConstructor = [](void* destPtr, const void* srcPtr)
				{
					const ComponentType* srcComponentPtr = reinterpret_cast<const ComponentType*>(srcPtr);
					new (destPtr) ComponentType(*srcComponentPtr);
				};

			componentTypeInfo->moveConstructor = [](void* destPtr, void* srcPtr)
				{

					ComponentType* srcComponentPtr = reinterpret_cast<ComponentType*>(srcPtr);

					new (destPtr) ComponentType(std::move(*srcComponentPtr));

				};

			m_componentIdToTypeInfoMap[m_nextComponentId] = std::move(componentTypeInfo);
			return m_nextComponentId++;
		}

	}



	template<typename ComponentType>
	ComponentId ComponentRegistry::getComponentIdFromComponent() const
	{
		using AbsoluteComponentType = std::decay_t<ComponentType>;//to reomove const ,pointer type etc
		std::type_index typeIndex = std::type_index(typeid(AbsoluteComponentType));

		auto it = m_typeIndexToComponentIdMap.find(typeIndex);

		if (it != m_typeIndexToComponentIdMap.end())
		{
			return it->second;
		}



		//TODO : NEED MORE THOUGHT HERE , should i allow new component to be registered and remove const

		throw std::runtime_error("Attempted to get ComponentId for unregistered type: "
			+ std::string(typeid(AbsoluteComponentType).name()));
		
		assert(0 && "Attempted to get ComponentId for an unregistered component type.");

		return 0;// registerComponent<AbsoluteComponentType>();
	}

}
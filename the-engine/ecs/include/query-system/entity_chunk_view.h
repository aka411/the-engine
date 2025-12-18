#pragma once

#include "common_data_types.h"
#include "component_registry.h"


namespace TheEngine::ECS
{


	class EntityChunkView
	{
	private:
		const ComponentRegistry& m_componentRegistry;

		size_t m_offsetIndex = 0;

		ArchetypeDefinition* m_archetypeDefinition = nullptr;
		std::uintptr_t m_chunkBaseAddress = 0;

	public:


		EntityChunkView(const EntityRecord& entityRecord, const ComponentRegistry& componentRegistry);


		template<typename ComponentType>
		ComponentType* getComponent();

	};



	template<typename ComponentType>
	ComponentType* EntityChunkView::getComponent()
	{
		ComponentId componentId = m_componentRegistry.getComponentIdFromComponent<ComponentType>();

		if (!m_archetypeDefinition->hasComponent(componentId))
		{
			return nullptr;
		}


		std::uintptr_t componentAddress = m_chunkBaseAddress + m_archetypeDefinition->GetComponentOffset(componentId) + sizeof(ComponentType) * m_offsetIndex;
		assert(componentAddress < m_archetypeDefinition->chunkRawSize + m_chunkBaseAddress);

		return reinterpret_cast<ComponentType*>(componentAddress);
	}


}
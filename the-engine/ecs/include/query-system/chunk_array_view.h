#pragma once
#include "common_data_types.h"
#include "component_registry.h"

namespace TheEngine::ECS
{

	class ChunkArrayView
	{
	private:
		const ComponentRegistry& m_componentRegistry;

		size_t m_entityCount = 0;
		size_t m_archetypeChunkSize = 0;

		ArchetypeDefinition* m_archetypeDefinition = nullptr;

		std::uintptr_t m_chunkBaseAddress = 0;

	public:

	

		ChunkArrayView(ArchetypeChunkHeader* archetypeChunkHeader, const ComponentRegistry& componentRegistry);

		const size_t getCount() const;



		template<typename ComponentType>
		ComponentType* getComponentArray();

	};



	template<typename ComponentType>
	ComponentType* ChunkArrayView::getComponentArray()
	{
		ComponentId componentId = m_componentRegistry.getComponentIdFromComponent<ComponentType>();

		std::uintptr_t componentAddress = m_chunkBaseAddress + m_archetypeDefinition->GetComponentOffset(componentId);
		assert(componentAddress < m_chunkBaseAddress + m_archetypeChunkSize);
		return reinterpret_cast<ComponentType*>(componentAddress);
	}




}
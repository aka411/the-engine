#pragma once

#include "../common_data_types.h"
#include "../component_registry.h"
#include "../entity_manager.h"
#include "../archetype_manager.h"
#include <stdint.h>
#include "query.h"
#include "entity_chunk_view.h"

namespace TheEngine::ECS
{




	class QuerySystem
	{

	private:

		 ComponentRegistry& m_componentRegistry;
		 EntityManager& m_entityManager;
		 ArchetypeManager& m_archetypeManager;

	public:
		QuerySystem(ComponentRegistry& componentRegistry, EntityManager& entityManager, ArchetypeManager& archetypeManager);
		

		template<typename... ComponentType>
		Query getQuery();

		EntityChunkView getEntityChunkView(const EntityId& entityId);

	
	
	};





	template<typename... ComponentType>
	inline Query QuerySystem::getQuery()
	{

		//process here 
		ArchetypeSignature neededArchtypeSignatureMask;

		(neededArchtypeSignatureMask.set(m_componentRegistry.getComponentIdFromComponent<ComponentType>()), ...);

		const std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>>& archetypeDefinitions = m_archetypeManager.getArchetypeDefinitions();

		//std::unordered_map<ArchetypeDefinition*, std::vector<ArchetypeChunkHeader*>> archetypeDefintionsToArchetypeChunkHeaders;
		Query query;


		for (const auto& it : archetypeDefinitions)
		{
			ArchetypeSignature archetypeSignature = it.first;

			if ((archetypeSignature & neededArchtypeSignatureMask) == neededArchtypeSignatureMask)
			{
				query.addArchetypeChunkHeaders(m_archetypeManager.getArchetypeChunksWithEntityData(archetypeSignature),m_componentRegistry);
			}

		}

		return query;
	}

}
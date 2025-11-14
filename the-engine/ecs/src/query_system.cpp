#include "query_system.h"
#include <ecs_manager.h>

namespace TheEngine::ECS
{
	QuerySystem::QuerySystem(ComponentRegistry& componentRegistry,EntityManager& entityManager, ArchetypeManager& archetypeManager) : m_componentRegistry(componentRegistry),m_entityManager(entityManager), m_archetypeManager(archetypeManager)
	{

	};

	EntityChunkView QuerySystem::getEntityChunkView(EntityId& entityId)
	{
		return EntityChunkView(m_entityManager.getEntityRecord(entityId));

	}

	void Query::addArchetypeChunkHeaders(std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders)
	{
		archetypeChunkHeaders.insert(archetypeChunkHeaders.end(), archetypeChunkHeaders.begin(), archetypeChunkHeaders.end());
	}


	ComponentRegistry& QuerySystem::getComponentRegistry()
	{
		return m_componentRegistry;
	};
};


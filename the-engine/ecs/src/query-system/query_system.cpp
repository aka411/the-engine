#include "query-system/query_system.h"


namespace TheEngine::ECS
{
	QuerySystem::QuerySystem(ComponentRegistry& componentRegistry,EntityManager& entityManager, ArchetypeManager& archetypeManager) : m_componentRegistry(componentRegistry),m_entityManager(entityManager), m_archetypeManager(archetypeManager)
	{

	};

	EntityChunkView QuerySystem::getEntityChunkView(const EntityId& entityId)
	{
		return EntityChunkView(m_entityManager.getEntityRecord(entityId),m_componentRegistry);

	}



};



#include "the-engine/ecs/entity_manager.h"
#include "the-engine/utils/logging/logger.h"





TheEngine::EntityId EntityManager::createEntity()
{
	{
		TheEngine::EntityId entityId;

		if (!m_availableEntityIds.empty())
		{
			entityId = m_availableEntityIds.top();
			m_availableEntityIds.pop();
			m_entities[entityId] = true;//set as active
		}
		else
		{
			entityId = m_entities.size();
			m_entities.push_back(true); // Initialize with a default value
		}
		return entityId;
	}
}

void EntityManager::destroyEntity(TheEngine::EntityId entityId)
{
	if (entityId >= m_entities.size() || !m_entities[entityId])//short-circuiting behavior prevents a potential out-of-bounds access to the entities
	{
		Logger::getInstance().report(LogLevel::Warning,"Tried to delete an non existing entity");
		return; // Entity does not exist or is already destroyed
	}
	m_entities[entityId] = false; // Mark the entity as destroyed
	m_availableEntityIds.push(entityId);//the entity id can be reused when needed
}

bool EntityManager::isAlive(TheEngine::EntityId entityId) const
{
	//short-circuiting behavior prevents a potential out-of-bounds access to the entities
	if (entityId >= m_entities.size() || !m_entities[entityId])
	{
		
		return false; // Entity does not exist or is already destroyed
	}

	return true; // Entity is alive
}

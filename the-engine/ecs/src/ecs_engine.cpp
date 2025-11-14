#include "ecs_engine.h"
#include <query_system.h>


namespace TheEngine::ECS
{
	ECSEngine::ECSEngine() :m_logger(), m_componentRegistry(), m_entityManager(), m_archetypeManager(m_logger,m_componentRegistry), m_ecsManager(m_componentRegistry, m_entityManager, m_archetypeManager), m_querySystem(m_componentRegistry, m_entityManager, m_archetypeManager)
	{

	}

	QuerySystem& ECSEngine::getQuerySystem()
	{
		return m_querySystem;
	}


	ECSManager& ECSEngine::getECSManager()
	{
		return m_ecsManager;
	}
}
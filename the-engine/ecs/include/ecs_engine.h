#pragma once
#include "ecs_manager.h"
#include "query_system.h"

namespace TheEngine::ECS
{
	class ECSEngine
	{

	private:
		 TheEngine::Utils::NullLogger m_logger;

		 ComponentRegistry m_componentRegistry;
		 EntityManager m_entityManager;
		 ArchetypeManager m_archetypeManager;


		 ECSManager m_ecsManager;
		 QuerySystem m_querySystem;

	public:
		ECSEngine();

		QuerySystem& getQuerySystem();
		ECSManager& getECSManager();

	};


}
#pragma once
#include "entity_manager.h"
namespace TheEngine::ECS
{
	class ECSManager
	{

	private:
		EntityManager m_entityManager;//ToDo : need more thought on this whether to DI
		ArchetypeManager m_archetypeManager;

	public:

		template<typename ComponentType>
		void addComponentToEntity(EntityId& entityId, ComponentType& component);



	};


	template<typename ComponentType>
	inline void ECSManager::addComponentToEntity(EntityId& entityId, ComponentType& component)
	{

		EntityRecord* entityRecord = m_entityManager.getEntityRecord(entityId);

		if (entityRecord == nullptr)
		{
			//ToDo: add logging here
			//log here
			return;
		}




		m_archetypeManager.addComponentToEntity<ComponentType>(entityRecord, component);

	}
}

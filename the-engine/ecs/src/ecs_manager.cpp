#include "ecs_manager.h"

namespace TheEngine::ECS
{


	ECSManager::ECSManager() :
		m_nullLogger(),
		m_componentRegistry(),
		m_entityManager(),
		m_archetypeManager(m_nullLogger,m_componentRegistry),
		m_commandBuffer(),
		m_commandProcessor(m_entityManager)
	{

	}
	

	EntityId ECSManager::createEntity()
	{
		return m_entityManager.createEntity();
	}


	void ECSManager::destroyEntity(EntityId& entityId)
	{
		Command command;
		command.commandType = CommandType::DESTROY_ENTITY;
		command.componentId = 0;
		command.ptr = nullptr;

		m_commandBuffer.storeCommand(entityId, command, nullptr);


	}


	void ECSManager::addComponntDataToEntity(std::vector<EntityAddInfo>& entityAddInfos)
	{


		for (const EntityAddInfo& entityAddInfo : entityAddInfos)
		{
			const std::vector<EntityRecordUpdate> EntityRecordUpdates = m_archetypeManager.addComponentToEntity(entityAddInfo);

			for (const EntityRecordUpdate& entityRecordUpdate : EntityRecordUpdates)
			{
				m_entityManager.updateEntityRecord(entityRecordUpdate);
			}

		}



	}


	void ECSManager::processDestructionOfEntities(std::vector<EntityId>& m_entityIdsToBeDestroyed)
	{

		for (const EntityId& entityId : m_entityIdsToBeDestroyed)
		{

			//ToDo : Need to write code here and in Archetype Manager also

		}



	}



}
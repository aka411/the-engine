#include "ecs_manager.h"

namespace TheEngine::ECS
{


	ECSManager::ECSManager(ComponentRegistry& componentRegistry, EntityManager& entityManager, ArchetypeManager& archetypeManager) :
		m_componentRegistry(componentRegistry),
		m_entityManager(entityManager),
		//m_nullLogger(),
		m_archetypeManager(archetypeManager),
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

	void  ECSManager::processCommands()
	{
		std::unordered_map<EntityId, std::vector<Command>>& entityIdToCommands = m_commandBuffer.getCommandList();
		m_commandProcessor.process(entityIdToCommands);
		std::vector<EntityAddInfo>& entityAddInfos = m_commandProcessor.getEntityAddInfos();

		addComponntDataToEntity(entityAddInfos);
		m_commandProcessor.reset();
		m_commandBuffer.reset();
	}

	void ECSManager::processDestructionOfEntities(std::vector<EntityId>& m_entityIdsToBeDestroyed)
	{

		for (const EntityId& entityId : m_entityIdsToBeDestroyed)
		{

			//ToDo : Need to write code here and in Archetype Manager also

		}



	}



}
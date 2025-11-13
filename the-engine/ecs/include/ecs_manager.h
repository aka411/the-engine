#pragma once
#include "ecs_manager.h"
#include <vector>
#include "component_registry.h"
#include "common_data_types.h"
#include "entity_manager.h"
#include "archetype_manager.h"
#include "command_buffer.h"
#include "command_processor.h"
#include "../../utils/include/logging/null_logger.h"


namespace TheEngine::ECS
{
	class ECSManager
	{
	private:
		//Current Design makes ECS manager owner of Archetype Manager, ComponentRegistry, Entity Manager
		TheEngine::Utils::NullLogger m_nullLogger;

		ComponentRegistry m_componentRegistry;
		EntityManager m_entityManager;
		ArchetypeManager m_archetypeManager;

		CommandBuffer m_commandBuffer;
		CommandProcessor m_commandProcessor;


		void addComponntDataToEntity(std::vector<EntityAddInfo>& m_entityAddInfos);
		void processDestructionOfEntities(std::vector<EntityId>& m_entityIdsToBeDestroyed);

	public:

		ECSManager();

		EntityId createEntity();
		void destroyEntity(EntityId& entityId);

		template<typename ComponentType>
		void addComponentToEntity(const EntityId& entityId, ComponentType& component);






		void processCommands();


		template<typename DataType>
		ComponentId registerComponent();

	};


	template<typename DataType>
	ComponentId ECSManager::registerComponent()
	{
		return m_componentRegistry.registerComponent<DataType>();
	}


	template<typename ComponentType>
	void ECSManager::addComponentToEntity(const EntityId& entityId, ComponentType& component)
	{
		Command command;
		command.commandType = CommandType::ADD_COMPONENT;
		command.componentId = m_componentRegistry.getComponentIdFromComponent<ComponentType>();
		command.ptr = &component;

		//owner if the data the pointer points to is Component Registry and is managed by unique pointer so no dangling pointers
		ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeInfo(command.componentId);
	
		m_commandBuffer.storeCommand(entityId, command, componentTypeInfo);

	}





}
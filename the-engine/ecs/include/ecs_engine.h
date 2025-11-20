#pragma once

#include <vector>
#include "component_registry.h"
#include "common_data_types.h"
#include "entity_manager.h"
#include "archetype_manager.h"
#include "command_buffer.h"
#include "command_processor.h"
#include "../../utils/include/logging/null_logger.h"
#include "query-system/query_system.h"


namespace TheEngine::ECS
{
	class ECSEngine
	{
	private:
	

		TheEngine::Utils::NullLogger m_logger;

		ComponentRegistry m_componentRegistry;
		EntityManager m_entityManager;
		ArchetypeManager m_archetypeManager;

		CommandBuffer m_commandBuffer;
		CommandProcessor m_commandProcessor;

		QuerySystem m_querySystem;


		void addComponntDataToEntity(std::vector<EntityAddInfo>& m_entityAddInfos);
		void processDestructionOfEntities(std::vector<EntityId>& m_entityIdsToBeDestroyed);

	public:

		ECSEngine();

		EntityId createEntity();
		void destroyEntity(EntityId& entityId);

		template<typename ComponentType>
		void addComponentToEntity(const EntityId& entityId, ComponentType& component);


		void processBufferedCommands();

		template<typename DataType>
		ComponentId registerComponent();




		/*Query System Area*/

		EntityChunkView getEntityChunkView(const EntityId& entityId);

		template<typename... ComponentType>
		inline Query getQuery();


	};




	template<typename DataType>
	ComponentId ECSEngine::registerComponent()
	{
		return m_componentRegistry.registerComponent<DataType>();
	}



	template<typename ComponentType>
	void ECSEngine::addComponentToEntity(const EntityId& entityId, ComponentType& component)
	{
		Command command;
		command.commandType = CommandType::ADD_COMPONENT;
		command.componentId = m_componentRegistry.getComponentIdFromComponent<ComponentType>();
		command.ptr = &component;

		//owner if the data the pointer points to is Component Registry and is managed by unique pointer so no dangling pointers
		ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeInfo(command.componentId);
	
		m_commandBuffer.storeCommand(entityId, command, componentTypeInfo);

	}


	template<typename... ComponentType>
	inline Query ECSEngine::getQuery()
	{

		return m_querySystem.getQuery<ComponentType...>();

	}


}
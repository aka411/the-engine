#pragma once  
#include "../../core/include/types.h"
#include "archetype_manager.h"
#include <stack>
#include "component_registry.h"

namespace TheEngine::ECS
{

	struct EntityId
	{/*
	 Note :
		
	 id == 0 and generation == 0 are used 
	 to represent invalid entity.
	 */
		
		size_t id = 0; // unique identifier for the entity
		uint16_t generation = 0; // generation number to handle entity reuse
	};


	struct EntityRecord
	{
		ArchetypeSignature archetypeSignature;
		uint16_t generation = 0;
		ArchetypeChunk* chunk = nullptr; // pointer to the chunk where the entity is located
		size_t chunkIndex = -1; // index of entities component in the specific chunk in the archetype
		//currently using size_t evaluate if I need negative ones
	};




	const EntityId INVALID_ENTITY_ID = {};






	class EntityManager
	{

	private:

		ArchetypeManager& m_archetypeManager;
		ComponentRegistry& m_componentRegistry;

		std::vector<EntityRecord> m_entityRegistry;
		
		std::stack<EntityId> m_freeEntityIds;//need better name 


	public:
		EntityManager() = default;
		~EntityManager() = default;

		void CreateEntity();

		void DestroyEntity(int entityId);


		template<typename DataType>
		void AddComponent(EntityId entityId, const DataType& component);
		void RemoveComponent(EntityId entityId, ComponentId componentTypeId);

		//template<typename DataType>
		//ComponentId  getComponentId ( const DataType& component) // just putting here if i ever need this


	};


	template<typename DataType>
	void AddComponent(EntityId entityId, const DataType& component)
	{
		//get current archetype of entity
		if (entityId.id > m_entityRegistry.size())
		{
			//handle invalid entity id 
			return;
		}

		EntityRecord& record = m_entityRegistry[entityId.id];

		if (record.generation != entityId.generation)
		{
			return; // handle invalid generation
		}

		//get componentId from component registry
		std::type_index typeIndex = typeid(DataType);

		ComponentId componentId = m_componentRegistry.getComponentId(typeIndex);
		if (componentId < 0)
		{
			//handle invalid component id
			return;
		}
		//if entity already has same component type then just replace it
		if (record.archetypeSignature.test(componentId))
		{
			//work from here
			record.chunk->archetypeDefinition->componentLayouts[componentId].moveConstructor(
				record.chunk->GetComponentDataPointer(record.chunkIndex, componentId),
				&component);

		}



		//if entity does not have component type then we need to add it to move it and add new data
			//need to move , and call necessary constructors
			//we also need to update entity records , archetype signature of entity
			//also we need to fill the gap or delete archtype chunl if its empty after move
		




	}
		



	


}
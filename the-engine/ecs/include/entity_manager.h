#pragma once  
#include "../../core/include/types.h"
#include "archetype_manager.h"
#include <stack>
#include "component_registry.h"

namespace TheEngine::ECS
{
	/*Invalid EntityId will be signaled by nullptr*/
	struct EntityId
	{

		size_t id = 0; // unique identifier for the entity
		uint16_t generation = 0; // generation number to handle entity reuse
	};

	/*Invalid Entity record will be signaled by nullptr*/
	struct EntityRecord
	{
		ArchetypeSignature& archetypeSignature;
		uint16_t generation = 0;
		ArchetypeChunk* chunk = nullptr; // pointer to the chunk where the entity is located
		size_t chunkIndex = 0; // index of entities component in the specific chunk in the archetype
		
	};










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

		EntityId* createEntity();
		void destroyEntity(EntityId& entityId);


		EntityRecord* getEntityRecord(const EntityId& entityId);//the caller has power to edit record
		//this is necesary to faciltate record update without having coupling with the archetype manager

		


	};









}
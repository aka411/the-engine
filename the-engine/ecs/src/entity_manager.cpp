
#include "entity_manager.h"
#include <common_data_types.h>
#include <cassert>


namespace TheEngine::ECS
{

	const EntityId EntityManager::createEntity()
	{
		
		if (!m_freeEntityIdStack.empty())
		{

			EntityId  reusedEntityId = m_freeEntityIdStack.top();
			m_freeEntityIdStack.pop();
			reusedEntityId.generation++;

			assert(reusedEntityId.id < m_entityRecordList.size());

			EntityRecord& reusedEntityRecord = m_entityRecordList[reusedEntityId.id];
			//entity record generation was updated in destroy method to casue a delibrate mismatch 
			reusedEntityRecord.entityId = reusedEntityId;
			reusedEntityRecord.archetypeSignature = EMPTY_ARCHETYPE_SIGNATURE;//reset this
			reusedEntityRecord.archetypeChunkHeader = nullptr;
			reusedEntityRecord.indexInArchetypeChunkRecordList = 0;

			return reusedEntityId;
		}
		

		EntityId newEntityId;
		newEntityId.id = m_entityRecordList.size();
		newEntityId.generation = 0;

		EntityRecord newEntityRecord;

		newEntityRecord.entityId = newEntityId;
		newEntityRecord.archetypeSignature = EMPTY_ARCHETYPE_SIGNATURE;//reset this
		newEntityRecord.archetypeChunkHeader = nullptr;
		newEntityRecord.indexInArchetypeChunkRecordList = 0;

		m_entityRecordList.push_back(newEntityRecord);

		return newEntityId;
	}

	const EntityRecord EntityManager::getEntityRecord(const EntityId& entityId) const
	{
		assert(entityId.id < m_entityRecordList.size());

		EntityRecord entityRecord = m_entityRecordList[entityId.id];

		assert(entityRecord.entityId.generation == entityId.generation);

		return entityRecord;

	}

	void EntityManager::updateEntityRecord(const EntityRecordUpdate& entityRecordUpdate)
	{
		assert(entityRecordUpdate.entityId.id < m_entityRecordList.size());

		EntityRecord& entityRecord  = m_entityRecordList[entityRecordUpdate.entityId.id];

		assert(entityRecord.entityId.id == entityRecordUpdate.entityId.id);
		assert(entityRecord.entityId.generation == entityRecordUpdate.entityId.generation);

		//what if its removed from archtype manager and needs signature update but archetypeChunkheader is nullptr
		assert(entityRecordUpdate.newArchetypeChunkHeader != nullptr);
		assert(entityRecordUpdate.newArchetypeChunkHeader->archetypeDefinition != nullptr);
		
		//entityRecord.archetypeSignature = entityRecordUpdate.newArchetypeChunkHeader->archetypeDefinition->archetypeSignature;
		entityRecord.archetypeChunkHeader = entityRecordUpdate.newArchetypeChunkHeader;
		entityRecord.indexInArchetypeChunkRecordList = entityRecordUpdate.newIndexInArchetypeChunkRecordList;



	}

	void EntityManager::destroyEntity(EntityId& entityId)
	{

		assert(entityId.id < m_entityRecordList.size());
		//not responsible for destroying its compoennts from archetype manager.

		EntityRecord& entityRecord = m_entityRecordList[entityId.id];

		assert(entityRecord.entityId.generation == entityId.generation);
		entityRecord.entityId.generation++;

		entityRecord.archetypeSignature = EMPTY_ARCHETYPE_SIGNATURE;//reset this
		entityRecord.archetypeChunkHeader = nullptr;
		entityRecord.indexInArchetypeChunkRecordList = 0;

		m_freeEntityIdStack.push(entityId);//gonna assume this is gonna make a copy , dont want dangling references

	}




}
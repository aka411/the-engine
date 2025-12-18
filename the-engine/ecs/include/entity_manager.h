#pragma once
#include <stack>
#include <vector>
#include "common_data_types.h"


namespace TheEngine::ECS
{

	class EntityManager
	{

	private:

		std::stack<EntityId> m_freeEntityIdStack;
		std::vector<EntityRecord> m_entityRecordList;
		
	public:

		const EntityId createEntity();

		const EntityRecord getEntityRecord(const EntityId& entityId) const;

		void updateEntityRecord(const EntityRecordUpdate& entityRecordUpdate);

		void destroyEntity(EntityId& entityId);

	};



}
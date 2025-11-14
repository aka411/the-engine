#pragma once
#include <vector>
#include <unordered_map>
#include "common_data_types.h"
#include "command_buffer.h"
#include "entity_manager.h"

namespace TheEngine::ECS
{
	//Note : The current implememntation will only do destroy Entity and add Component To entity

	class CommandProcessor
	{

	private:

		std::vector<EntityAddInfo> m_entityAddInfos;
		std::vector<EntityId> m_entityIdsToBeDestroyed;

		EntityManager& m_entityManager;

	public:
		CommandProcessor(EntityManager& entityManager);
		void process(std::unordered_map<EntityId, std::vector<Command>>& entityIdToBufferedCommandMap);
		std::vector<EntityAddInfo>& getEntityAddInfos();
		void reset();
	};

}
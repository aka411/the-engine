#pragma once
#include <unordered_map>
#include "common_data_types.h"
#include "i_component_registry.h"
#include "bump_allocator.h"



namespace std
{
	template <>
	struct hash<TheEngine::ECS::EntityId>
	{
		size_t operator()(const TheEngine::ECS::EntityId& entityId) const
		{
			// Need more thought here
			size_t h1 = std::hash<size_t>{}(entityId.id);
			size_t h2 = std::hash<size_t>{}(entityId.generation);
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
}

namespace TheEngine::ECS
{
	enum class CommandType
	{
		ADD_COMPONENT,
		REMOVE_COMPONENT,
		DESTROY_ENTITY,
		UNKNOWN

	};

	struct Command
	{
		CommandType commandType = CommandType::UNKNOWN;
		ComponentId componentId = 0;
		void* ptr = nullptr;

	};

	class CommandBuffer
	{

	private:


		std::unordered_map<EntityId, std::vector<Command>, std::hash<EntityId>> m_entityIdToBufferedCommandMap;

		BumpAllocator m_bumpAllocator;

	public:

		CommandBuffer();

		void storeCommand(const EntityId& entityid, const Command& command, const ComponentTypeInfo* const componentTypeInfo);
		std::unordered_map<EntityId, std::vector<Command>>& getCommandList();

		void reset();
	};



}
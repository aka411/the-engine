#include "command_buffer.h"
#include <cassert>

namespace TheEngine::ECS
{
	CommandBuffer::CommandBuffer()
	{

	}
	void CommandBuffer::storeCommand(const EntityId& entityid, const Command& command, const ComponentTypeInfo* const componentTypeInfo)
	{
		//should i check generation?
		std::vector<Command>& commands = m_entityIdToBufferedCommandMap[entityid];//uses custom hashing


		Command localCommand;
		if (command.commandType == CommandType::ADD_COMPONENT)
		{
			void* destPtr = m_bumpAllocator.allocateAligned(componentTypeInfo->size, componentTypeInfo->alignment);
			if (destPtr == nullptr)
			{
				//ToDo : add correct handling
				assert(0);
			}


		
			localCommand.componentId = command.componentId;
		

			//moving from stack to our local managed memory block
			void* srcPtr = command.ptr;

			assert(srcPtr != nullptr);
			assert(componentTypeInfo != nullptr);

			componentTypeInfo->moveConstructor(destPtr, srcPtr);

			localCommand.ptr = destPtr;




		}
		localCommand.commandType = command.commandType;
		commands.push_back(localCommand);


	}

	std::unordered_map<EntityId, std::vector<Command>>& CommandBuffer::getCommandList()
	{

		return m_entityIdToBufferedCommandMap;
	}




	void CommandBuffer::reset()
	{
		m_entityIdToBufferedCommandMap.clear();
	}
}
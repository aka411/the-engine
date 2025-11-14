#include "command_processor.h"
#include <cassert>

namespace TheEngine::ECS
{
    CommandProcessor::CommandProcessor(EntityManager& entityManager): m_entityManager(entityManager)
    {


    }


    void CommandProcessor::process(std::unordered_map<EntityId, std::vector<Command>>& entityIdToBufferedCommandMap)
    {

        
        m_entityAddInfos.reserve(entityIdToBufferedCommandMap.size());

        for (const auto& pair : entityIdToBufferedCommandMap)
        {
            const EntityId& entityId = pair.first;
            const std::vector<Command>& commands = pair.second;

            EntityAddInfo entityAddInfo;
            entityAddInfo.entityRecord = m_entityManager.getEntityRecord(entityId);
            ArchetypeSignature newArchetypeSignature = 0;

            for (const Command& command : commands)
            {
                if (command.commandType == CommandType::ADD_COMPONENT)
                {
                    newArchetypeSignature.set(command.componentId);

                    ComponentData componentData;
                    componentData.componentId = command.componentId;
                    componentData.ptr = command.ptr;
  
                    entityAddInfo.componentDataList.push_back(componentData);
                }
                else if (command.commandType == CommandType::REMOVE_COMPONENT)
                {
                    //TODO : IMPLEMENT THIS
                    assert(0);
                }
                else if (command.commandType == CommandType::DESTROY_ENTITY)
                {
                    //TODO : Need more thought here
                    //will some one add then destroy?
                    assert(commands.size() == 1);
                    m_entityIdsToBeDestroyed.push_back(entityId);
                    break;
                }
                else if (command.commandType == CommandType::UNKNOWN)
                {
                    //ToDo : Handle this better
                    assert(0);
                }


            }

            if (entityAddInfo.componentDataList.size() > 0)
            {
                entityAddInfo.newArchetypeSignature = newArchetypeSignature;
                m_entityAddInfos.push_back(entityAddInfo);
            }
        }


    }

    std::vector<EntityAddInfo>& CommandProcessor::getEntityAddInfos()
    {
        return m_entityAddInfos;
    }

    void CommandProcessor::reset()
    {
        m_entityAddInfos.resize(0);
        m_entityIdsToBeDestroyed.resize(0);

    }
}
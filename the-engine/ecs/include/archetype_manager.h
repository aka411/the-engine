#pragma once 

namespace TheEngine::ECS
{

struct ArchetypeDefinition
{

}



    class ArchetypeManager
    {
        private:
            std::unordered_map<ComponentMask, std::unique_ptr<ArchetypeDefinition>> m_archetypeDefinitions;
        public:

        
    }

}
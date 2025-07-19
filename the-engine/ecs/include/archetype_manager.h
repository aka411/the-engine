#pragma once 

namespace TheEngine::ECS
{



using ComponentId =  std::bitset<MAX_COMPONENTS>;
using ComponentFilter =  std::bitset<MAX_COMPONENTS>;
using ArchetypeSignature =  std::bitset<MAX_COMPONENTS>;

struct  ComponentType
{


}


struct ComponentLayout
{
    componentId id = 0;

    uint32_t size = 0;
    uint32_t alignment = 0;

    size_t offsetInChunk = 0;

     ComponentType* componentType;




}

struct ArchetypeDefinition
{
     ArchetypeSignature archetypeSignature;



}


class ArchetypeChunk
{
    private:

        uint32_t m_numOfEntities = 0;

        ArchetypeDefinition* archetypeDefinition;


    public:  
    
    



}


struct Chunklist
{
    //owner of memory region is allocator
    std::vector<ArchetypeChunk*> fullChunks;
    std::vector<ArchetypeChunk*>  availableChunks; 
}


    class ArchetypeManager
    {
        private:
            std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>> m_archetypeDefinitions;


        public:

        
    }

}
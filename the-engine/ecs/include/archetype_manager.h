#pragma once 
#include <unordered_map>
#include <memory>
#include <bitset>
#include "chunk_pool_allocator.h"


namespace TheEngine::ECS
{

	constexpr size_t CHUNK_RAW_SIZE = 16 * 1024; 
	constexpr size_t MAX_COMPONENTS = 10;
	using ComponentId = int16_t;

	using ComponentFilter = std::bitset<MAX_COMPONENTS>;
	using ArchetypeSignature = std::bitset<MAX_COMPONENTS>;




	struct ComponentLayout
	{
		ComponentId id = 0;

		uint32_t size = 0; //redundant already in ComponentTypeinfo
		uint32_t alignment = 0;//redundant already in ComponentTypeinfo

		size_t offsetInChunk = 0; //offset in chunk memory region

		ComponentTypeInfo* componentTypeInfo;


	};



	struct ArchetypeDefinition
	{
		ArchetypeSignature archetypeSignature;

		//should be ordered by alignment then size
		std::vector<ComponentLayout> componentLayouts;


		size_t chunkRawSize = 0; //size of the chunk in bytes
		//including header and if  padding present also

		size_t chunkEntityCapacity = 0; //how many entities can be stored for this chunkRawSize

		

	};


	struct ArchetypeChunk
	{
		ArchetypeDefinition* archetypeDefinition = nullptr;


		//The below data could be put inside ArchetypeDefinition
		size_t chunkRawSize = 0; //size of the chunk in bytes
		//including header and if  padding present also

		size_t chunkEntityCapacity = 0; //how many entities can be stored in this chunk

		size_t chunkEntityUsed = 0; //how many entities are currently stored in this chunk	

	};

	struct ChunkList
	{
		//owner of memory region is allocator
		std::vector<ArchetypeChunk*> fullChunks;
		std::vector<ArchetypeChunk*>  availableChunks;
	};


	class ArchetypeManager
	{
	private:
		
		ComponentRegistry& m_componentRegistry;
		ChunkPoolAllocator& m_chunkPoolAllocator;

		std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>> m_archetypeDefinitions;
		std::unordered_map<ArchetypeSignature, ChunkList> m_chunkLists;

	private:


		bool checkFit(const size_t numOfEntities, const size_t chunkRawSize, const size_t archetypeHeaderSize, std::vector<ComponentLayout>& componentLayouts);
		
		void CategorizeChunks(ChunkList& chunkList);

	public:

		ArchetypeManager() = default;
		~ArchetypeManager() = default;


		ArchetypeDefinition* GetOrCreateArchetypeDefinition(const ArchetypeSignature& signature);
		ArchetypeChunk* GetOrCreateChunk(const ArchetypeSignature& signature);




	};

}
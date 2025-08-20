#pragma once 
#include <unordered_map>
#include <memory>
#include <bitset>
#include "chunk_pool_allocator.h"


class EntityRecord;


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



	//maps offset to entity id
	//ArchetypeChunk is the sole owner of this
	struct ArchetypeRecordChunk
	{
		EntityId id ;
	};



	struct ArchetypeChunk
	{
		ArchetypeDefinition* archetypeDefinition = nullptr;//not the owner

		ArchetypeRecordChunk* archetypeRecord = nullptr;//owner

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
		
		void categorizeChunks(ChunkList& chunkList);


		ArchetypeDefinition* getOrCreateArchetypeDefinition(const ArchetypeSignature& signature);
		ArchetypeChunk* getOrCreateFreeArchetypeChunk(const ArchetypeSignature& signature);


	public:

		ArchetypeManager() = default;
		~ArchetypeManager() = default;



		template<typename ComponentType>
		void addComponentToEntity(EntityRecord& entityRecord, const ComponentType& component);

		void moveBetweenArchetypes(ArchetypeChunk* const srcArchetypeChunk, ArchetypeChunk* const destArchetypeChunk, );
		//ToDo : check const correctness
		//       complete method signature as required







	


	};



	template<typename ComponentType>
	inline void addComponentToEntity(EntityRecord& entityRecord, const ComponentType& component)//called by ECS manager
	{
		//ToDo : Decide whether to replace reference with pointer and adding nullptr check


		//ToDo : Add logic to add component to Entity
			//1)Get component id
			//2)Get current Archetype  and compare if already there
			// 
			// 
				//2.a) if already there then replace the component only
				// 
				//2.b)if not there already need to move to new archetypechunk and update records

		//Also handle other cases.
		



		//get CompoentId
		ComponentId componentId = m_componentRegistry.getComponentId(std::type_index(typeid(ComponentType)));


		//check id
		if (!entityRecord.archetypeSignature.test(componentId))
		{
			/*move between archetypes*/




		}
		else
		{
			/*replacing component*/




		}




	};



}
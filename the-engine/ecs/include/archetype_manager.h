#pragma once
#include <vector>
#include <bitset>
#include <unordered_map>
#include <memory>
#include "../../utils/include/logging/i_logger.h"
#include "component_registry.h"
#include "common_data_types.h"

namespace TheEngine::ECS
{

	//chunk raw size might not be needed if we go with max entities per chunk approach
	constexpr size_t CHUNK_RAW_SIZE = 16 * 1024;
	



	struct ComponentLayout
	{
		ComponentId componentId = 0;

		uint32_t size = 0; //redundant already in ComponentTypeinfo
		uint32_t alignment = 0;//redundant already in ComponentTypeinfo

		size_t offsetInChunk = 0; //offset in chunk memory region

		ComponentTypeInfo* componentTypeInfo;


	};



	struct ArchetypeDefinition
	{
		ArchetypeSignature archetypeSignature;

		//should be sorted by alignment then size
		std::vector<ComponentLayout> componentLayouts;


		size_t chunkRawSize = 0;//total size of ArchetypeChunk the data is stored in for which the offsets are calculated

		size_t chunkMaxEntityCapacity = 0; //how many entities can be stored for this chunkRawSize



	};


	struct ArchetypeRecordChunk
	{
		//ToDo : need to hashout the alignment requirements here when storing in chunk
		size_t maxCount = 0; //max entities ids that can be stored in this chunk
		//this maxCount should actually be in Archetype header but since we cannot use only id[] FAM we need here to avoid zero size array
		EntityId id[];
		

	};


	struct ArchetypeChunk
	{

		//uint8_t data[1]; //raw data block ,hacky way to define a flexible array member in C++
		//Gonna use empty struct cause flexible array member is not allowed in C++
	
		
	};


	struct ArchetypeChunkHeader
	{
		ArchetypeDefinition* archetypeDefinition = nullptr;//not the owner

		ArchetypeRecordChunk* archetypeRecordChunk = nullptr;//owner
		ArchetypeChunk* archetypeChunk = nullptr; //owner

		size_t chunkEntityUsed = 0; //how many entities are currently stored in this chunk	


	};



	struct ChunkList
	{
		//owner of memory region is allocator
		std::vector<ArchetypeChunkHeader*> fullChunks;
		std::vector<ArchetypeChunkHeader*>  availableChunks;
	};



	class ArchetypeManager
	{


	private:


		TheEngine::Utils::ILogger& m_logger;



		static const size_t MAX_NUM_OF_ENTITIES_PER_CHUNK = 256; // adjust based on performance testing


		std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>> m_archetypeDefinitions;
		std::unordered_map<ArchetypeSignature, ChunkList> m_archetypeChunksMap;

		ComponentRegistry& m_componentRegistry;

		/*
		struct ComponentLayout
		{
			ComponentId componentId = 0;

			//consider removing these after checking if its used anywhere
			uint32_t size = 0; //redundant already in ComponentTypeinfo
			uint32_t alignment = 0;//redundant already in ComponentTypeinfo

			size_t offsetInChunk = 0; //offset in chunk memory region

			ComponentTypeInfo* componentTypeInfo;


		};
		*/

		//Methods:
		bool moveArchetypeChunkHeaderToAvailableList(ArchetypeChunkHeader* archetypeChunkHeader);
		bool moveArchetypeChunkHeaderToFullList(ArchetypeChunkHeader* archetypeChunkHeader);



		bool moveEntityData(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityRecord* entityRecord, std::vector<EntityRecordUpdate>& entityRecordUpdateList);


		std::vector<EntityRecordUpdate>  transferEntityBetweenArchetype(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityAddInfo entityAddInfo);

		EntityRecordUpdate addComponentDataToArchetypeChunk(ArchetypeChunkHeader* archetypeChunkHeader,EntityAddInfo entityAddInfo);


		ArchetypeDefinition* createNewArchetypeDefinition(const ArchetypeSignature& archetypeSignature);

		ArchetypeChunkHeader* createArchetypeChunkHeader(ArchetypeDefinition* archetypeDefinition);

		ArchetypeChunkHeader* createOrGetArchetypeChunk(ArchetypeSignature archetypeSignature);



	public:
		ArchetypeManager();
		~ArchetypeManager();


		//ToDo : Implement ArchetypeManager

		// Public Methods:
		//	 3 Public Methods: addComponentToEntity(), deleteEntityData(), getArchetypeChunks()

		// 1 public Method(New): removeComponentFromEntity() // for MVP we are not gonna implement this



		/*
		struct ComponentData
		{

		ComponentId componentId;

		void* data; //pointer to component data in contiguous memory block

		}
		*/

		/*
		struct EntityRecord
		{

		EntityId id;

		ArchetypeSignature& archetypeSignature;
		ArchetypeChunk* archetypeChunk; //pointer to the chunk where entity data is stored
		uint32_t index; //index of the entity within the chunk



		}
		
		*/





		/*
		struct EntityAddInfo
		{

		EntityRecord entityRecord;
		std::vector<ComponentData> componentDataList;

		}
		*/

		std::vector<EntityRecordUpdate> addComponentToEntity(EntityAddInfo entityAddInfo);
		/*
		struct EntityRecordUpdate
		{
		EntityId id;
		ArchetypeChunk* archetypeChunk;
		uint32_t offset;
		}
		*/



		//std::vector<EntityRecordUpdate> removeComponentFromEntity(EntityRemoveInfo entityAddInfo);//ToDo : Need more thought here
		//For MVP we are not gonna implement this


		std::vector<EntityRecordUpdate> deleteEntityData(EntityRecord* entityRecord);//Entity record null check will be performed by the orchastrator



		std::vector<ArchetypeChunk*> getArchetypeChunks(ArchetypeSignature archetypeSignature);




	};

}
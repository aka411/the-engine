#pragma once
#include <vector>
#include <bitset>
#include <unordered_map>
#include <memory>
#include "../../utils/include/logging/i_logger.h"
#include "component_registry.h"

namespace TheEngine::ECS
{


	constexpr size_t CHUNK_RAW_SIZE = 16 * 1024;
	constexpr size_t MAX_COMPONENTS = 10;
	using ComponentId = int16_t;

	using ComponentFilter = std::bitset<MAX_COMPONENTS>;
	using ArchetypeSignature = std::bitset<MAX_COMPONENTS>;





	struct ArchetypeDefinition
	{
		ArchetypeSignature archetypeSignature;

		//should be sorted by alignment then size
		std::vector<ComponentLayout> componentLayouts;


		size_t chunkRawSize = 0; //size of the chunk in bytes
		//including header and if  padding present also

		size_t chunkEntityCapacity = 0; //how many entities can be stored for this chunkRawSize



	};


	struct ArchetypeRecordChunk
	{
		//ToDo : need to hashout the alignment requirements here when storing in chunk
		size_t maxCount = 0; //max entities ids that can be stored in this chunk
		EntityId id[];
		

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

		std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>> m_archetypeDefinitions;
		std::unordered_map<ArchetypeSignature, ChunkList> m_archetypeChunksMap;

		ComponentRegistry& m_componentRegistry;

		/*
		struct ComponentLayout
		{
			ComponentId componentId = 0;

			uint32_t size = 0; //redundant already in ComponentTypeinfo
			uint32_t alignment = 0;//redundant already in ComponentTypeinfo

			size_t offsetInChunk = 0; //offset in chunk memory region

			ComponentTypeInfo* componentTypeInfo;


		};
		*/

		//Methods:

		bool moveEntityData(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityRecord* entityRecord, EntityRecordUpdate& entityRecordUpdate);


		std::vector<EntityRecordUpdate>  transferEntityBetweenArchetype(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityAddInfo entityAddInfo);

		EntityRecordUpdate addComponentDataToArchetypeChunk(ArchetypeChunkHeader* archetypeChunkHeader,EntityAddInfo entityAddInfo);


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



		//std::vector<EntityRecordUpdate> removeComponentToEntity(EntityRemoveInfo entityAddInfo);//ToDo : Need more thought here
		//For MVP we are not gonna implement this


		std::vector<EntityRecordUpdate> deleteEntityData(EntityRecord* entityRecord);//Entity record null check will be performed by the orchastrator



		std::vector<ArchetypeChunk*> getArchetypeChunks(ArchetypeSignature archetypeSignature);




	};

}
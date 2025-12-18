#pragma once
#include <vector>
#include <bitset>
#include <unordered_map>
#include <memory>
#include "../../utils/include/logging/i_logger.h"
#include "i_component_registry.h"
#include "common_data_types.h"

namespace TheEngine::ECS
{

	//chunk raw size not be needed if we go with max entities per chunk approach
	//constexpr size_t CHUNK_RAW_SIZE = 16 * 1024;
	



	struct ChunkList
	{
		//owner of memory region is allocator
		//full chunks will be full of data
		//in availabe chunk top might have data
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

		IComponentRegistry& m_componentRegistry;

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
		bool moveArchetypeHeaderChunkToCorrectList(std::vector<ArchetypeChunkHeader*>& destChunkList, std::vector<ArchetypeChunkHeader*>& srcChunkList, ArchetypeChunkHeader* archetypeChunkHeader);
		

		bool moveArchetypeChunkHeaderToAvailableList(ArchetypeChunkHeader* archetypeChunkHeader);
		bool moveArchetypeChunkHeaderToFullList(ArchetypeChunkHeader* archetypeChunkHeader);



		struct MoveResult
		{
			bool moveResult = false;
			EntityRecordUpdate movedEntityRecordUpdate;

			bool entityMovedToFillGap = false;
			EntityRecordUpdate entityMovedToFillGapRecordUpdate;
		};

		MoveResult moveEntityData(ArchetypeChunkHeader* destArchetypeChunkHeader, ArchetypeChunkHeader* srcArchetypeChunkHeader, EntityRecord* entityToBeMovedRecord);


		std::vector<EntityRecordUpdate>  transferAndAddEntityDataToArchetypeChunk(ArchetypeChunkHeader* destArchetypeChunkHeader, ArchetypeChunkHeader* srcArchetypeChunkHeader, EntityAddInfo entityAddInfo);

		EntityRecordUpdate addInitialComponentDataToArchetypeChunk(ArchetypeChunkHeader* archetypeChunkHeader,EntityAddInfo entityAddInfo);


		ArchetypeDefinition* createNewArchetypeDefinition(const ArchetypeSignature& archetypeSignature);

		ArchetypeChunkHeader* createArchetypeChunkHeader(ArchetypeDefinition* archetypeDefinition);

		ArchetypeChunkHeader* createOrGetArchetypeChunk(ArchetypeSignature archetypeSignature);



	public:
		ArchetypeManager(TheEngine::Utils::ILogger& logger, IComponentRegistry& componentRegistry);
		~ArchetypeManager();


		std::vector<EntityRecordUpdate> addComponentToEntity(EntityAddInfo entityAddInfo);




		//std::vector<EntityRecordUpdate> removeComponentFromEntity(EntityRemoveInfo entityAddInfo);//ToDo : Need more thought here
		//For MVP we are not gonna implement this


		//std::vector<EntityRecordUpdate> deleteEntityData(EntityRecord* entityRecord);//Entity record null check will be performed by the orchastrator


		const std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>>& getArchetypeDefinitions() const;
		std::vector<ArchetypeChunkHeader*>  getArchetypeChunksWithEntityData(ArchetypeSignature archetypeSignature);




	};

}
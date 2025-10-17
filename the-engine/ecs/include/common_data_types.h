#pragma once
#include <cstdint>



namespace TheEngine::ECS
{
	//forward declaration

	struct ArchetypeChunkHeader;



	using ComponentId = int16_t;
	constexpr size_t MAX_COMPONENTS = 10;
	using ComponentFilter = std::bitset<MAX_COMPONENTS>;
	using ArchetypeSignature = std::bitset<MAX_COMPONENTS>;




	struct EntityId
	{

		size_t id = 0; // unique identifier for the entity
		uint16_t generation = 0; // generation number to handle entity reuse
	};




	struct ComponentData
	{

		ComponentId componentId;

		void* data; //pointer to component data in contiguous memory block

	};


	struct EntityRecord
	{

		EntityId id;

		ArchetypeSignature& archetypeSignature;
		ArchetypeChunkHeader* archetypeChunkHeader; //pointer to the chunk where entity data is stored
		uint32_t index; //index of the entity within the chunk



	};


	struct EntityAddInfo
	{

		EntityRecord entityRecord;
		std::vector<ComponentData> componentDataList;

	};


	struct EntityRecordUpdate
	{
		EntityId id;
		ArchetypeChunkHeader* newArchetypeChunkHeader;
		uint32_t newIndex;
	};

}
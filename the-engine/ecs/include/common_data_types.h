#pragma once
#include <cstdint>
#include <bitset>
#include <functional>



namespace TheEngine::ECS
{
	






	using ComponentId = int16_t;
	constexpr size_t MAX_COMPONENTS = 100;
	using ComponentFilter = std::bitset<MAX_COMPONENTS>;
	using ArchetypeSignature = std::bitset<MAX_COMPONENTS>;


	constexpr ArchetypeSignature EMPTY_ARCHETYPE_SIGNATURE = 0;


	struct EntityId
	{

		size_t id = 0; // unique identifier for the entity
		uint16_t generation = 0; // generation number to handle entity reuse

		bool operator==(const EntityId& other) const
		{
			// Two EntityIds are the same ONLY if both their ID and generation match.
			return id == other.id && generation == other.generation;
		}
	};




	struct ComponentData
	{

		ComponentId componentId;

		void* ptr; //pointer to component data in contiguous memory block

	};


	struct ComponentTypeInfo
	{




		ComponentId componentId = 0;

		size_t size = 0;
		size_t alignment = 0;

		std::function<void(void*)> constructor = nullptr; // Function to construct the component
		std::function<void(void*)> destructor = nullptr; // Function to destruct the component

		//dest , src
		std::function<void(void*, const void*)> copyConstructor = nullptr; // Function to copy construct the component
		std::function<void(void*, void*)> moveConstructor = nullptr; // Function to move construct the component

	};

	struct ComponentLayout
	{
		ComponentId componentId = 0;

		uint32_t size = 0; //redundant already in ComponentTypeinfo
		uint32_t alignment = 0;//redundant already in ComponentTypeinfo

		size_t offsetInChunk = 0; //offset in chunk memory region

		ComponentTypeInfo* componentTypeInfo = nullptr;


	};


	struct ArchetypeDefinition
	{
		ArchetypeSignature archetypeSignature;

		//should be sorted by alignment then size
		//A map or unordered map would be better
		std::vector<ComponentLayout> componentLayouts;
		std::unordered_map<ComponentId, size_t>  componentIdToComponentOffset;

		size_t chunkRawSize = 0;//total size of ArchetypeChunk the data is stored in for which the offsets are calculated

		size_t chunkMaxEntityCapacity = 0; //how many entities can be stored for this chunkRawSize

		bool hasComponent(ComponentId componentTypeId) const
		{
			return archetypeSignature.test(componentTypeId);
		}

		size_t GetComponentOffset(ComponentId componentTypeId) const
		{
			return componentIdToComponentOffset.at(componentTypeId);
		}
	};

	struct ArchetypeChunk
	{

		//uint8_t data[1]; //raw data block ,hacky way to define a flexible array member in C++
		//Gonna use empty struct cause flexible array member is not allowed in C++


	};


	struct ArchetypeRecordChunk
	{
		//ToDo : need to hashout the alignment requirements here when storing in chunk
		//size_t maxCount = 0; //max entities ids that can be stored in this chunk
		//this maxCount should actually be in Archetype header but since we cannot use only id[] FAM we need here to avoid zero size array
		EntityId id[];


	};

	struct ArchetypeChunkHeader
	{
		ArchetypeDefinition* archetypeDefinition = nullptr;//not the owner

		ArchetypeRecordChunk* archetypeRecordChunk = nullptr;//owner
		ArchetypeChunk* archetypeChunk = nullptr; //owner

		size_t chunkEntityUsed = 0; //how many entities are currently stored in this chunk	


	};



	struct EntityRecord
	{

		EntityId entityId;

		ArchetypeSignature archetypeSignature = EMPTY_ARCHETYPE_SIGNATURE;
		ArchetypeChunkHeader* archetypeChunkHeader = nullptr; //pointer to the chunk where entity data is stored
		uint32_t indexInArchetypeChunkRecordList =  0; //index of the entity within the chunk



	};


	struct EntityAddInfo
	{

		EntityRecord entityRecord;
		ArchetypeSignature newArchetypeSignature = EMPTY_ARCHETYPE_SIGNATURE;
		std::vector<ComponentData> componentDataList;

	};


	struct EntityRecordUpdate
	{
		EntityId entityId;
		ArchetypeChunkHeader* newArchetypeChunkHeader = nullptr;
		uint32_t newIndexInArchetypeChunkRecordList = 0;
	};

}
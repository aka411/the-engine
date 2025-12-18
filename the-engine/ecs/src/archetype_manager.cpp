
#include <vector>
#include <memory>
#include <cassert>
#include "../include/archetype_manager.h"
#include "../../utils/include/logging/logger_macro.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace TheEngine::ECS
{
	using namespace TheEngine::Utils;

	//Futre work : clean up
	//Error Handling : We are gonna do strict error handling in this class as any error here can cause silent data corruption,
	//                 any unexpected beahavior should be handled by shutting down the engine and logging the error



	bool ArchetypeManager::moveArchetypeHeaderChunkToCorrectList(std::vector<ArchetypeChunkHeader*>& destChunkList, std::vector<ArchetypeChunkHeader*>& srcChunkList, ArchetypeChunkHeader* archetypeChunkHeader)
	{
		//REVIEW : DONE


		if (archetypeChunkHeader == nullptr)
		{
			assert(archetypeChunkHeader != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is nullptr in moveArchetypeHeaderChunkToCorrectList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here

		}

		//we need to find the archetypeChunk Header pointer to be moved in srcChunkList
		for (size_t i = 0; i < srcChunkList.size(); ++i)
		{
			if (srcChunkList[i] == archetypeChunkHeader)
			{
				//Found the chunk header in full list, move it to available list
				destChunkList.push_back(archetypeChunkHeader);

				//overwrite the the copied pointer with last element and pop back to avoid gaps in vector
				srcChunkList[i] = srcChunkList.back();
				srcChunkList.pop_back();//size correction


				return true;
			}
		}

		//failed to find the ArchetypeChunkHeader in source means that this archetypeChunkHeader is a leak or archetypeChunkHeader pointer is wrong 
		return false;
	}




	bool ArchetypeManager::moveArchetypeChunkHeaderToAvailableList(ArchetypeChunkHeader* archetypeChunkHeader)
	{
		//REVIEW : DONE

		if (archetypeChunkHeader == nullptr)
		{
			//if this happens something went wrong somewhere before this call meaning some code failed to handle error properly
			//ToDo : Write better message
			assert(archetypeChunkHeader != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is nullptr in moveArchetypeChunkHeaderToAvailableList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here
		}



		const  ArchetypeDefinition* archetypeDefinition = archetypeChunkHeader->archetypeDefinition;

		if (archetypeDefinition == nullptr)
		{
			assert(archetypeDefinition != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeDefinition is nullptr in moveArchetypeChunkHeaderToAvailableList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here

		}


		const ArchetypeSignature& signature = archetypeDefinition->archetypeSignature;



		auto it = m_archetypeChunksMap.find(signature);

		if (it != m_archetypeChunksMap.end())
		{
			//we have found the ChunkList for this signature

			ChunkList& chunkList = it->second;

			//for convenience we make references to the vectors in chunkList, this is just to make code more readable
			std::vector<ArchetypeChunkHeader*>& fullChunks = chunkList.fullChunks;
			std::vector<ArchetypeChunkHeader*>& availableChunks = chunkList.availableChunks;



			bool moveResult = moveArchetypeHeaderChunkToCorrectList(availableChunks, fullChunks, archetypeChunkHeader);

			if (!moveResult)
			{
				assert(moveResult);
				LOG_ERROR(m_logger, LogSource::ECS, "Failed to move ArchetypeChunkHeader To AvailableList");

				//TODO : Call engine shutdown and error handling here as this is a serious error

				return false;//code excecution should never reach here

			}

			return true;
		}
		else
		{
			//No signature found, this means the archetype chunk is not managed by this manager and its a leak or maybe a wrong address due to memory corruption

			//The best way to handle this is shutdown cause this should never happen , all resources should have been properly managed
			assert(it != m_archetypeChunksMap.end());
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeSignature not found in moveArchetypeChunkHeaderToAvailableList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here
		}

		//If we reach here, something went wrong
		return false;//code excecution should never reach here
	}

	bool ArchetypeManager::moveArchetypeChunkHeaderToFullList(ArchetypeChunkHeader* archetypeChunkHeader)
	{

		//REVIEW : DONE



		if (archetypeChunkHeader == nullptr)
		{
			//if this happens something went wrong somewhere before this call meaning some code failed to handle error properly
			//ToDo : Write better message
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is nullptr in moveArchetypeChunkHeaderToFullList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here
		}

		//TODO : add null check on ArchetypeDefinition
		const  ArchetypeDefinition* archetypeDefinition = archetypeChunkHeader->archetypeDefinition;

		if (archetypeDefinition == nullptr)
		{

			assert(archetypeDefinition != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeDefinition is nullptr in moveArchetypeChunkHeaderToFullList");

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;//code excecution should never reach here

		}


		const ArchetypeSignature& signature = archetypeDefinition->archetypeSignature;

		auto it = m_archetypeChunksMap.find(signature);

		if (it != m_archetypeChunksMap.end())
		{
			ChunkList& chunkList = it->second;

			std::vector<ArchetypeChunkHeader*>& fullChunks = chunkList.fullChunks;
			std::vector<ArchetypeChunkHeader*>& availableChunks = chunkList.availableChunks;


			bool moveResult = moveArchetypeHeaderChunkToCorrectList(fullChunks, availableChunks, archetypeChunkHeader);

			if (!moveResult)
			{

				assert(moveResult);
				LOG_ERROR(m_logger, LogSource::ECS, "Failed to move ArchetypeChunkHeader To FullList");

				//TODO : Call engine shutdown and error handling here as this is a serious error

				return false;//code excecution should never reach here
			}


			//success
			return true;
		}
		else
		{
			assert(it != m_archetypeChunksMap.end());
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeSignature not found in moveArchetypeChunkHeaderToAvailableList");

			//TODO : Call engine shutdown and error handling here as this is a serious error
			return false;//ToDo : change to proper error handling as this is gonna cause a silent corruption
		}

		//If we reach here, something went wrong
		return false;

	}




	ArchetypeManager::MoveResult ArchetypeManager::moveEntityData(ArchetypeChunkHeader* destArchetypeChunkHeader, ArchetypeChunkHeader* srcArchetypeChunkHeader, EntityRecord* entityToBeMovedRecord)
	{

		//REVIEW : In Progress

		//Aim : To move data of entity from sourceArchetypeChunk to destArchetypeChunk
		//CAN HANDLE BOTH UPSHIFT AND DOWNSHIFT AS THIS METHOD ONLY COPIES WHAT 
		// DESTNATION ACCEPTS BUT WE STILL NEED ANOTHER METHOD TO ADD EXTRA COMPONENTS THAT SOURCE DID NOT HAVE



		MoveResult moveResult;
		moveResult.moveResult = false;


		if (srcArchetypeChunkHeader == nullptr)
		{
			assert(srcArchetypeChunkHeader != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Source ArchetypeChunkHeader is nullptr in moveEntityData");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			return moveResult;
		}

		if (destArchetypeChunkHeader == nullptr)
		{
			assert(destArchetypeChunkHeader != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Destination ArchetypeChunkHeader is nullptr in moveEntityData");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			return moveResult;
		}

		if (entityToBeMovedRecord == nullptr)
		{
			assert(entityToBeMovedRecord != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "EntityRecord is nullptr in moveEntityData");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			return moveResult;

		}



		if (srcArchetypeChunkHeader->chunkEntityUsed == 0)//means there is no entity in source 
		{
			assert(srcArchetypeChunkHeader->chunkEntityUsed != 0);
			LOG_ERROR(m_logger, LogSource::ECS, "Corruption in srcArchetypeChunkHeader detected,chunkEntityUsed in srcArchetypeChunkHeader cannot be zero");
			//TODO : Call engine shutdown and error handling here as this is a serious error

			return moveResult;//move is false but move actually happened

		}



		//ToDo : Review and  move logic and gap filling logic
		//ToDo : check all pointer arthemetic and logic




		//Aim : Transfer exsisting data , delete left data  ,send out commands to update both records to entity manager
		//Notes : After this method call Entity Record is invalid and should not be used anymore untill updated by entity manager which will only happen after external call to entity manager with the returned EntityRecordUpdate

		const ArchetypeDefinition* srcArchetypeDefinition = srcArchetypeChunkHeader->archetypeDefinition;


		const ArchetypeDefinition* destArchetypeDefinition = destArchetypeChunkHeader->archetypeDefinition;


		const std::vector<ComponentLayout>& srcComponentLayouts = srcArchetypeDefinition->componentLayouts;
		const std::vector<ComponentLayout>& destComponentLayouts = destArchetypeDefinition->componentLayouts;


		const size_t numOfSrcComponents = srcComponentLayouts.size();
		const size_t numOfDestComponents = destComponentLayouts.size();


		ArchetypeChunk* srcArchetypeChunk = srcArchetypeChunkHeader->archetypeChunk;
		ArchetypeChunk* destArchetypeChunk = destArchetypeChunkHeader->archetypeChunk;


		ArchetypeRecordChunk* srcArchetypeRecordChunk = srcArchetypeChunkHeader->archetypeRecordChunk;
		ArchetypeRecordChunk* destArchetypeRecordChunk = destArchetypeChunkHeader->archetypeRecordChunk;




		/****** CODE TO MOVE ENTITY FROM SORUCE TO DESTINATION ARCHETYPECHUNK*******/

		std::uintptr_t srcArchetypeChunkBaseAddress = reinterpret_cast<std::uintptr_t>(srcArchetypeChunk);
		std::uintptr_t destArchetypeChunkBaseAddress = reinterpret_cast<std::uintptr_t>(destArchetypeChunk);

		//const ArchetypeSignature& srcArchetypeSignature = srcArchetypeDefinition->archetypeSignature;

		const size_t srcToBeMovedEntityIndex = entityToBeMovedRecord->indexInArchetypeChunkRecordList;
		const size_t destFreeEntitySlotIndex = destArchetypeChunkHeader->chunkEntityUsed;// zero based index

		for (auto& destComponentLayout : destComponentLayouts)
		{

			for (auto& srcComponentLayout : srcComponentLayouts)
			{
				if (srcComponentLayout.componentId == destComponentLayout.componentId)
				{

					//TODO : WATCH OUT FOR ONE OFF ERROR
					std::uintptr_t srcComponentAddress = srcArchetypeChunkBaseAddress + srcComponentLayout.offsetInChunk + (srcToBeMovedEntityIndex * srcComponentLayout.componentTypeInfo->size);//null check?
					std::uintptr_t destComponentAddress = destArchetypeChunkBaseAddress + destComponentLayout.offsetInChunk + (destFreeEntitySlotIndex * destComponentLayout.componentTypeInfo->size);//null check?

					//move

					void* srcComponentPtr = reinterpret_cast<void*>(srcComponentAddress);
					void* destComponentPtr = reinterpret_cast<void*>(destComponentAddress);

					//moving to dest from src
					destComponentLayout.componentTypeInfo->moveConstructor(destComponentPtr, srcComponentPtr);

				}

			}

		}


		/***LOOP TO DESTRUCT LEFT OVER COMPONENTS IN SOURCE CHUNK***/

		for (auto& srcComponentLayout : srcComponentLayouts)
		{


			std::uintptr_t srcComponentAddress = srcArchetypeChunkBaseAddress + srcComponentLayout.offsetInChunk + (srcToBeMovedEntityIndex * srcComponentLayout.componentTypeInfo->size);

			void* srcComponentPtr = reinterpret_cast<void*> (srcComponentAddress);

			//destruct
			srcComponentLayout.componentTypeInfo->destructor(srcComponentPtr);

		}



		/******CODE TO FILL LEFT GAP IN SOURCE ARCHETYPECHUNK*******/
		//This code is doing work only on srcArchetypeChunk



		const size_t srcLastEntityIndex = srcArchetypeChunkHeader->chunkEntityUsed - 1;//minus one cause if dont it is the next free slot
		const size_t srcGapEntityIndex = entityToBeMovedRecord->indexInArchetypeChunkRecordList;


		if (srcLastEntityIndex != 0 && srcGapEntityIndex != srcLastEntityIndex) //  what we moved was not the last and there is atleast one more entity in this ArchetypeChunk
		{




			for (auto& srcComponentLayout : srcComponentLayouts)
			{


				std::uintptr_t srcComponentAddress = srcArchetypeChunkBaseAddress + srcComponentLayout.offsetInChunk + (srcLastEntityIndex * srcComponentLayout.componentTypeInfo->size);
				std::uintptr_t destComponentAddress = srcArchetypeChunkBaseAddress + srcComponentLayout.offsetInChunk + (srcGapEntityIndex * srcComponentLayout.componentTypeInfo->size);

				void* srcComponentPtr = reinterpret_cast<void*> (srcComponentAddress);
				void* destComponentPtr = reinterpret_cast<void*> (destComponentAddress);

				//move
				srcComponentLayout.componentTypeInfo->moveConstructor(destComponentPtr, srcComponentPtr);

				//destruct // no need actually
				srcComponentLayout.componentTypeInfo->destructor(srcComponentPtr);

			}




			// update last entity record to point to new index, update archetype record accordingly
			srcArchetypeRecordChunk->id[srcGapEntityIndex] = srcArchetypeRecordChunk->id[srcLastEntityIndex];

			EntityRecordUpdate lastEntityRecordUpdate;

			lastEntityRecordUpdate.entityId = srcArchetypeRecordChunk->id[srcLastEntityIndex];
			lastEntityRecordUpdate.newArchetypeChunkHeader = srcArchetypeChunkHeader;// TODO : fix needed ,we have to move ArchetypeHeader to common data type 
			lastEntityRecordUpdate.newIndexInArchetypeChunkRecordList = srcGapEntityIndex;

			moveResult.entityMovedToFillGap = true;

			moveResult.entityMovedToFillGapRecordUpdate = lastEntityRecordUpdate;

		}




		//Now handle destination chunk header

					//update number of entities
		srcArchetypeChunkHeader->chunkEntityUsed--;
		destArchetypeChunkHeader->chunkEntityUsed++;


		destArchetypeRecordChunk->id[destFreeEntitySlotIndex] = entityToBeMovedRecord->entityId;


		if (destArchetypeChunkHeader->chunkEntityUsed == MAX_NUM_OF_ENTITIES_PER_CHUNK)
		{
			//move to full list logic here
			bool isMoved = moveArchetypeChunkHeaderToFullList(destArchetypeChunkHeader);


			assert(isMoved);
			LOG_ERROR(m_logger, LogSource::ECS, "Failed to move destArchetypeChunkHeader to FullList");
			//TODO : Call engine shutdown and error handling here as this is a serious error

			return moveResult;//move is false but move actually happened

		}

		if (srcArchetypeChunkHeader->chunkEntityUsed == MAX_NUM_OF_ENTITIES_PER_CHUNK - 1)
		{
			bool isMoved = moveArchetypeChunkHeaderToAvailableList(srcArchetypeChunkHeader);


			assert(isMoved);
			LOG_ERROR(m_logger, LogSource::ECS, "Failed to move srcArchetypeChunkHeader to AvaialbleList");
			//TODO : Call engine shutdown and error handling here as this is a serious error

			return moveResult;//move is false but move actually happened
		}




		EntityRecordUpdate destEntityRecordUpdate;

		destEntityRecordUpdate.entityId = entityToBeMovedRecord->entityId;
		destEntityRecordUpdate.newArchetypeChunkHeader = destArchetypeChunkHeader;
		destEntityRecordUpdate.newIndexInArchetypeChunkRecordList = destFreeEntitySlotIndex;


		moveResult.movedEntityRecordUpdate = destEntityRecordUpdate;

		//move validation
		moveResult.moveResult = true;


		return moveResult;


	}

	std::vector<EntityRecordUpdate> ArchetypeManager::transferAndAddEntityDataToArchetypeChunk(ArchetypeChunkHeader* destArchetypeChunkHeader, ArchetypeChunkHeader* srcArchetypeChunkHeader, EntityAddInfo entityAddInfo)
	{


		//Aim : To transfer data  between srcArchetypeChunkHeader and destArchetypeChunkHeader then add data in  entityAddInfo
		//transfer exsisting data , delete left data or add new data , update both records ;

		std::vector<EntityRecordUpdate> entityRecordUpdateList;

		MoveResult moveResult = moveEntityData(destArchetypeChunkHeader, srcArchetypeChunkHeader, &entityAddInfo.entityRecord);

		if (!moveResult.moveResult)
		{
			assert(moveResult.moveResult);
			LOG_ERROR(m_logger, LogSource::ECS, "Failed to transfer entityData to destinationArchetypeChunk");
			//TODO : Call engine shutdown and error handling here as this is a serious error

			return entityRecordUpdateList;
		}



		//TODO : ADD NULL CHECKS
		std::vector<ComponentLayout>& destComponentLayouts = destArchetypeChunkHeader->archetypeDefinition->componentLayouts;
		ArchetypeChunk* destArchetypeChunk = destArchetypeChunkHeader->archetypeChunk;

		std::uintptr_t destChunkBaseAddress = reinterpret_cast<std::uintptr_t>(destArchetypeChunk);

		const size_t movedEntityIndex = moveResult.movedEntityRecordUpdate.newIndexInArchetypeChunkRecordList;

		for (ComponentData& componentData : entityAddInfo.componentDataList)
		{

			for (ComponentLayout& destComponentLayout : destComponentLayouts)
			{

				if (destComponentLayout.componentId == componentData.componentId)
				{
					std::uintptr_t destComponentAddress = destChunkBaseAddress + destComponentLayout.offsetInChunk + (movedEntityIndex * destComponentLayout.componentTypeInfo->size);

					void* destComponentPtr = reinterpret_cast<void*>(destComponentAddress);
					//move construct
					destComponentLayout.componentTypeInfo->moveConstructor(destComponentPtr, componentData.ptr);

					//call destructor on source data 
					//Note : Actually not needed as we moved data from src chunk in moveEntityData so no source data exsists for these new components
					//Also all these src data is in temp memory block which will be freed by caller
					destComponentLayout.componentTypeInfo->destructor(componentData.ptr);

					break; //break inner loop as we found the component
				}
			}

		}

		entityRecordUpdateList.push_back(moveResult.movedEntityRecordUpdate);

		if (moveResult.entityMovedToFillGap)
		{
			entityRecordUpdateList.push_back(moveResult.entityMovedToFillGapRecordUpdate);
		}

		return entityRecordUpdateList;

	}



	EntityRecordUpdate ArchetypeManager::addInitialComponentDataToArchetypeChunk(ArchetypeChunkHeader* archetypeChunkHeader, EntityAddInfo entityAddInfo)
	{

		//REVIEW : In progress (95%)

		//AIM : This method is to initially add the component data and no need to move

		if (archetypeChunkHeader == nullptr)
		{
			assert(archetypeChunkHeader != nullptr);
			LOG_ERROR(m_logger, TheEngine::Utils::LogSource::ECS, "ArchetypeChunkHeader is null in addComponentDataToArchetypeChunk");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			//CODE EXECUTION STOPS HERE

			return EntityRecordUpdate{};
		}



		ArchetypeChunk* archetypeChunk = archetypeChunkHeader->archetypeChunk;
		if (archetypeChunk == nullptr)
		{
			assert(archetypeChunk != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunk is nullptr in addComponentDataToArchetypeChunk");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			//CODE EXECUTION STOPS HERE

			return EntityRecordUpdate{};
		}


		ArchetypeRecordChunk* archetypeRecordChunk = archetypeChunkHeader->archetypeRecordChunk;
		if (archetypeRecordChunk == nullptr)
		{
			assert(archetypeRecordChunk != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeRecordChunk is nullptr in addComponentDataToArchetypeChunk");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			//CODE EXECUTION STOPS HERE

			return EntityRecordUpdate{};
		}


		const ArchetypeDefinition* archetypeDefinition = archetypeChunkHeader->archetypeDefinition;
		if (archetypeDefinition == nullptr)
		{
			assert(archetypeDefinition != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeDefinition is nullptr in addComponentDataToArchetypeChunk");
			//TODO : Call engine shutdown and error handling here as this is a serious error
			//CODE EXECUTION STOPS HERE

			return EntityRecordUpdate{};
		}


		const std::vector<ComponentLayout>& destComponentLayouts = archetypeDefinition->componentLayouts;//from archetypehCunkHeader
		//TODO : do i need to check for zero size of destComponentLayouts ,most probaly not

		//ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeInfo(0);
		
		std::uintptr_t archetypeChunkBaseAddress = reinterpret_cast<std::uintptr_t>(archetypeChunk);
		const size_t entityDestIndex = archetypeChunkHeader->chunkEntityUsed;

		std::vector<ComponentData>& componentDataList = entityAddInfo.componentDataList;

		// To check if the data transfer is within bounds of ArchetypeChunk  for safety, but should never happen if logic is correct
		const size_t archetypeChunkRawSize = archetypeDefinition->chunkRawSize;
		const std::uintptr_t archetypeChunkEndAddress = archetypeChunkBaseAddress + archetypeChunkRawSize;


		for (ComponentData& srcComponentData : componentDataList)
		{
			for (const ComponentLayout& destComponentLayout : destComponentLayouts)
			{
				if (srcComponentData.componentId == destComponentLayout.componentId)
				{
					//TODO : need to review this
					size_t destComponentAddress = archetypeChunkBaseAddress + destComponentLayout.offsetInChunk + (entityDestIndex * destComponentLayout.componentTypeInfo->size);



					if ((archetypeChunkEndAddress) < (destComponentAddress + destComponentLayout.componentTypeInfo->size)) // safety check
					{
						assert((archetypeChunkEndAddress) > (destComponentAddress + destComponentLayout.componentTypeInfo->size));
						LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunk OVERFLOW detected during component data addition. ");
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE

						return EntityRecordUpdate{};
					}



					//move construct
					destComponentLayout.componentTypeInfo->moveConstructor(reinterpret_cast<void*>(destComponentAddress), srcComponentData.ptr);



					// call destructor on source data for safety
					//what if there is a bad move constructor, wont that resource be lost?
					//Note : Actually not needed as we moved data from src chunk in moveEntityData so no source data exsists for these new components
					//Also all these src data is in temp memory block which will be freed by caller

					destComponentLayout.componentTypeInfo->destructor(srcComponentData.ptr);
					break; //break inner loop as we found the component and find next in source
				}
			}
		}

		//Update Records in ArchetypeChunk Header and its relevant records

		archetypeChunkHeader->chunkEntityUsed++;
		//check if full and if full move to full chunk list

		archetypeRecordChunk->id[entityDestIndex] = entityAddInfo.entityRecord.entityId;

		//TODO : The maxcount in ArchetypeChunkRecord might get moved to header itself so be wary of design change
		if (archetypeChunkHeader->chunkEntityUsed == MAX_NUM_OF_ENTITIES_PER_CHUNK)
		{
			//move to full list logic here

			bool isMoved = moveArchetypeChunkHeaderToFullList(archetypeChunkHeader);

			assert(isMoved);
			LOG_ERROR(m_logger, LogSource::ECS, "Failed to move ArchetypeChunkHeader to fullList in ChunkList ");
			//TODO : Call engine shutdown and error handling here as this is a serious error 
			//CODE EXECUTION STOPS HERE

		}

		EntityRecordUpdate entityRecordUpdate;

		entityRecordUpdate.entityId = entityAddInfo.entityRecord.entityId;
		entityRecordUpdate.newArchetypeChunkHeader = archetypeChunkHeader;
		entityRecordUpdate.newIndexInArchetypeChunkRecordList = entityDestIndex;

		return entityRecordUpdate;


	}

	ArchetypeDefinition* ArchetypeManager::createNewArchetypeDefinition(const ArchetypeSignature& archetypeSignature)
	{
		//REVIEW : In progress(90 percent Complete)

		//Aim : To create new ArchetypeDefinition based on given ArchetypeSignature

		//This method interacts with ComponentManager to get component type info for each component in the signature
		std::vector<ComponentTypeInfo*> componentTypeInfos;

		//ToDo : The below loop is very inefficient as it loops  MAX_COMPONENTS times, a better way would be to find the
		//       first set bit and then next set bit and so on Or find first set bit and proceed from there

		// Method A-1 vvvvvvvv: Loops through whole bits  
		for (ComponentId componentId = 0; componentId < MAX_COMPONENTS; ++componentId)
		{
			if (archetypeSignature.test(componentId))
			{

				//owner of componentTypeInfo is ComponentRegistry
				ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeInfo(componentId);

				if (componentTypeInfo == nullptr)
				{
					assert(componentTypeInfo != nullptr);
					LOG_ERROR(m_logger, LogSource::ECS, "ComponentTypeInfo not found for componentId in createNewArchetypeDefinition");
					//TODO : Call engine shutdown and error handling here as this is a serious error
					//CODE EXECUTION STOPS HERE

					return nullptr;
				}

				componentTypeInfos.push_back(componentTypeInfo);


			}
		}
		// Method A-1 ^^^^^^^^^^^: Loops through whole bits  




		// Method A-2 vvvvvv: First get MSB then loop from there

		//Research into this has led me to the conclusion that a single cycle method to find MSB does not exsist for std::bitset
		// Method A-2 ^^^^^^^: First get MSB then loop from there



		//we need to sort componentTypeInfos by alignment then size for proper memory layout
		//in Descending order of alignment 
		std::sort(componentTypeInfos.begin(), componentTypeInfos.end(), [](const ComponentTypeInfo* a, const ComponentTypeInfo* b)
			{
				if (a->alignment == b->alignment)
				{
					return a->size > b->size; //larger size first if same alignment
				}
				return a->alignment > b->alignment; //larger alignment first
			});



		//Note : Calculating total chunk size required for a given number of entities is more memory efficient than calculating max entities for a given chunk size

		//IMPORANT NOTE : The calculation assumes all components
		//				  alignement requirements are power of two and sorted from bigger to smaller alignment 
		//                requirement so we dont need padding , and the allocated chunk start 
		//				  address is aligned for the largest alignemnt requirement in for the list of components in archetype. 
		// 
		// This also assumes compiler inserts padding internally in structs to satisfy 
		// alignment requirements else this ecs chunk system is not gonna work properly
		//                  



		//ToDo : Calculate offsets and total chunk size required from componentTypeInfos and given number of entities

		//ALOGRITHM :
		  //Get total size (totalComponentsSize)required for one entity by summing up sizes of all components
		  //Get required size of chunk for given number of entities (requiredChunkSize) by multiplying totalComponentsSize with number of entities 
		  //calculate offsets for each component in componentTypeInfos


		size_t totalComponentsSize = 0;

		for (size_t i = 0; i < componentTypeInfos.size(); ++i)
		{
			totalComponentsSize += componentTypeInfos[i]->size;
		}




		//create component layouts with offsets
		size_t currentOffset = 0;
		std::vector<ComponentLayout> componentLayouts;
		std::unordered_map<ComponentId, size_t> componentIdToComponentOffset;
		for (size_t i = 0; i < componentTypeInfos.size(); ++i)
		{
			ComponentLayout layout;
			layout.componentId = componentTypeInfos[i]->componentId;
			layout.size = componentTypeInfos[i]->size;
			layout.alignment = componentTypeInfos[i]->alignment;
			layout.offsetInChunk = currentOffset;
			layout.componentTypeInfo = componentTypeInfos[i];
			componentIdToComponentOffset[layout.componentId] = layout.offsetInChunk;
			currentOffset += componentTypeInfos[i]->size * MAX_NUM_OF_ENTITIES_PER_CHUNK; //increment offset by size * max entities to get next component offset

			componentLayouts.push_back(layout);
		}

		size_t requiredTotalChunkSize = MAX_NUM_OF_ENTITIES_PER_CHUNK * totalComponentsSize;//in bytes


		std::unique_ptr<ArchetypeDefinition> newArchetypeDefinition = std::make_unique<ArchetypeDefinition>();

		newArchetypeDefinition->archetypeSignature = archetypeSignature;
		newArchetypeDefinition->componentLayouts = componentLayouts;
		newArchetypeDefinition->chunkRawSize = requiredTotalChunkSize;
		newArchetypeDefinition->componentIdToComponentOffset = componentIdToComponentOffset;

		//store in map

		m_archetypeDefinitions[archetypeSignature] = std::move(newArchetypeDefinition);

		// get newly created and stored ArchetypeDefinition and returns a pointer to it

		const auto& it = m_archetypeDefinitions.find(archetypeSignature);

		if (it != m_archetypeDefinitions.end())
		{
			return it->second.get();//pointer to Archetype Definition
		}
		else
		{
			assert(it != m_archetypeDefinitions.end());
			LOG_ERROR(m_logger, LogSource::ECS, "Newly created ArchetypeDefinition not found in ArchetypeSignature-ArchetypeDefinition Map");
			//TODO : Call engine shutdown and error handling here as this is a serious error

			return nullptr;//code execution should not reach here, if it reaches here that means engine shutdown failed
		}




		return nullptr;//code execution should not reach here
	}

	ArchetypeChunkHeader* ArchetypeManager::createArchetypeChunkHeader(ArchetypeDefinition* archetypeDefinition)
	{
		//REVIEW : Done
		//Aim : To allocate a new ArchetypeChunkHeader along with ArchetypeChunk and ArchetypeRecordChunk from ArchetypeDefinition

		void* archetypeChunkHeaderRawPtr = _aligned_malloc( sizeof(ArchetypeChunkHeader), alignof(ArchetypeChunkHeader));

		if (archetypeChunkHeaderRawPtr == nullptr)
		{
			    assert(archetypeChunkHeaderRawPtr != nullptr);
				LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader");
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE
		}


		//TODO : Repalce with correct alignment requirement
		//TODO : Handling of zeroSignature, this is a bug found during testing
		assert(archetypeDefinition->componentLayouts.size() > 0);
		void* archetypeChunkRawPtr = _aligned_malloc(archetypeDefinition->chunkRawSize, archetypeDefinition->componentLayouts[0].alignment);
		
		if (archetypeChunkRawPtr == nullptr)
		{
			_aligned_free(archetypeChunkHeaderRawPtr);//TODO : these are fragile need better way to do this 
			assert(archetypeChunkRawPtr != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunk");
				//TODO : Call engine shutdown and error handling here as this is a serious error
	            //CODE EXECUTION STOPS HERE
			
		}

		void* ArchetypeRecordChunkRawPtr = _aligned_malloc(MAX_NUM_OF_ENTITIES_PER_CHUNK*sizeof(EntityId), alignof(ArchetypeRecordChunk));
		
		if (ArchetypeRecordChunkRawPtr == nullptr)
		{
			_aligned_free(archetypeChunkHeaderRawPtr);
			_aligned_free(archetypeChunkRawPtr);
			assert(ArchetypeRecordChunkRawPtr != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeRecordChunk");
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE
			
		}


		//All allocations were successful

		//casting them to correct type
		//ArchetypeChunkHeader* newArchetypeChunkHeader = static_cast<ArchetypeChunkHeader*>(archetypeChunkHeaderRawPtr);

		ArchetypeChunkHeader* newArchetypeChunkHeader = new (archetypeChunkHeaderRawPtr) ArchetypeChunkHeader();

		ArchetypeChunk* newArchetypeChunk = static_cast<ArchetypeChunk*>(archetypeChunkRawPtr);
		ArchetypeRecordChunk* newArchetypeRecordChunk = static_cast<ArchetypeRecordChunk*>(ArchetypeRecordChunkRawPtr);


	
		newArchetypeChunkHeader->archetypeDefinition = archetypeDefinition;

		//Linking them ,think of it as a binary tree with ArchetypeChunkHeader as parent and ArchetypeChunk and ArchetypeRecord as two child of it.
		newArchetypeChunkHeader->archetypeChunk = newArchetypeChunk;
		newArchetypeChunkHeader->archetypeRecordChunk = newArchetypeRecordChunk;

		//we only need to return it , storing will be handled by caller
		return newArchetypeChunkHeader;
	}




	ArchetypeChunkHeader* ArchetypeManager::createOrGetArchetypeChunk(ArchetypeSignature archetypeSignature)
	{
		//REVIEW : Done

		//try to find existing ChunkList for this archetypeSignature
		auto it = m_archetypeChunksMap.find(archetypeSignature);


		//The next stpe can go in one of two ways
			//1. ChunkList of ArchetypeSiganture already exists 
			//2. ChunkList of ArchetypeSiganture does not  exists and we may need to create the ChunkList and ArchetypeDefinition

		if (it != m_archetypeChunksMap.end())//1. ChunkList of ArchetypeSiganture already exists 
		{



			ChunkList& chunkList = it->second;

			//The next stpe can go in one of two ways
			//1. There is atleast one ArchetypeChunkHeader with atleast one free slot in availableChunks in ChunkList for the Archetype
			//2. The availableChunks for the Archetype is empty and need to create new chunks and put it in the availableChunks in ChunkList
			if (!chunkList.availableChunks.empty())//1. There is atleast one ArchetypeChunkHeader with atleast one free slot in availableChunks in ChunkList for the Archetype.
			{
				//Return the first available chunk
				return chunkList.availableChunks.back();

			}
			else//2. The availableChunks for the Archetype is empty and need to create new chunks and put it in the availableChunks in ChunkList
			{



				ArchetypeDefinition* archetypeDef = m_archetypeDefinitions[archetypeSignature].get();


				if (archetypeDef == nullptr)
				{
					assert(archetypeDef != nullptr);
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to get archetypeDef , there is corruption somewhere or need to review logic in Chunklist and archetype lifetime relations ");
						//TODO : Call engine shutdown and error handling here as this is a serious error

						//code excecution stops here cause if there is 
						//exisiting ChunkList for a ArchetypeSignature then there must be a ArchetypeDefinition for it
						return nullptr;
				}


				// create new ArchetypeChunkHeader

				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(archetypeDef);

				if (newArchetypeChunkHeader == nullptr)
				{
					assert(newArchetypeChunkHeader != nullptr);
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader");
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						return nullptr;
				}


				//This is an address we are pushing into vector 
				chunkList.availableChunks.push_back(newArchetypeChunkHeader);

				return newArchetypeChunkHeader;
			}
		}
		else//2. ChunkList of ArchetypeSiganture does not  exists and we may need to create the ChunkList and ArchetypeDefinition
		{




			//check if ArchetypeDefinition already exsists
			auto defIt = m_archetypeDefinitions.find(archetypeSignature);


			//The next stpe can go in one of two ways
			//1. ArchetypeDefinition exsists and we dont need to create the ArchetypeDefinition for signature and use it to get ArchetypeChunkHeader,also we need to create ChunkList for this Archetype
			//2. ArchetypeDefinition does not exsist so we need to create ArchetypeDefinition from ArchetypeSignature  , ChunkList for it , and create and put a ArchetypeChunkHeader into availableList in ChunkList
			if (defIt != m_archetypeDefinitions.end())			//1. It exsists and we dont need to create the ArchetypeDefinition for signature and use it to get ArchetypeChunkHeader,also we need to create ChunkList for this Archetype
			{

				//create new ChunkList from definition
				ChunkList newChunkList;

				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(defIt->second.get());

				if (newArchetypeChunkHeader == nullptr)
				{
					assert(newArchetypeChunkHeader != nullptr);
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader");
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						return nullptr;
				}

				//Inserting pointer into vector
				newChunkList.availableChunks.push_back(newArchetypeChunkHeader);

				m_archetypeChunksMap[archetypeSignature] = newChunkList;//since ChunkList is just two std::vectors of pointers we can copy or move

				return newArchetypeChunkHeader;

			}
			else//2. ArchetypeDefinition does not exsist so we need to create ArchetypeDefinition from ArchetypeSignature  , ChunkList for it , and create and put a ArchetypeChunkHeader into availableList in ChunkList
			{

				//call to create new ArchetypeDefinition based on Archetype Signature
				ArchetypeDefinition* newArchetypeDefinition = createNewArchetypeDefinition(archetypeSignature);


				if (newArchetypeDefinition == nullptr)
				{
					assert(newArchetypeDefinition != nullptr);
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeDefinition for signature");
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						return nullptr;
				}





				//Create new ArchetypeChunk
				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(newArchetypeDefinition);

				if (newArchetypeChunkHeader == nullptr)
				{
					assert(newArchetypeChunkHeader != nullptr);
						LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader");
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						return nullptr;
				}




				//Create new ChunkList
				ChunkList newChunkList;


				newChunkList.availableChunks.push_back(newArchetypeChunkHeader);

			
				//copy or move ChunkList to map casue its just two vectors of pointers so safe
				m_archetypeChunksMap[archetypeSignature] = newChunkList;


				return newArchetypeChunkHeader;

			}

		}


	}



		/****Public Methods ****/


	ArchetypeManager::ArchetypeManager(ILogger& logger, IComponentRegistry& componentRegistry) : m_logger(logger), m_componentRegistry(componentRegistry)
		{
			//ToDo : Implement Constructor
		}

		ArchetypeManager::~ArchetypeManager()
		{
			
			//very fragile code , review carefully
			for (auto& pair : m_archetypeChunksMap)
			{
				ChunkList& chunkList = pair.second;
				for (ArchetypeChunkHeader* chunkHeader : chunkList.fullChunks)
				{
					_aligned_free(chunkHeader->archetypeChunk);
					_aligned_free(chunkHeader->archetypeRecordChunk);
					_aligned_free(chunkHeader);
				}

				for (ArchetypeChunkHeader* chunkHeader : chunkList.availableChunks)
				{
					_aligned_free(chunkHeader->archetypeChunk);
					_aligned_free(chunkHeader->archetypeRecordChunk);
					_aligned_free(chunkHeader);
				}
			}





		}




		std::vector<EntityRecordUpdate> ArchetypeManager::addComponentToEntity(EntityAddInfo entityAddInfo)
		{
			//  REVIEW : Done

			ArchetypeSignature newEntityArchetypeSignature = entityAddInfo.newArchetypeSignature;

			//Add up all component ids and make up the new ArchetypeSignature 
			//for (auto& componentData : entityAddInfo.componentDataList)
			//{
			//	newEntityArchetypeSignature.set(componentData.componentId);
			//}



			//The next stpe can go in one of two ways
			 //1. Initial add of data to archetype
			 //2. Moving from old archetype to new archetype

			if (entityAddInfo.entityRecord.archetypeChunkHeader == nullptr) // 1. Initial add of data to archetype
			{



				//get a archetypeChunkHeader with atleast one slot free for adding data
				ArchetypeChunkHeader* archetypeChunkHeader = createOrGetArchetypeChunk(newEntityArchetypeSignature);

				if (archetypeChunkHeader == nullptr)
				{
					assert(archetypeChunkHeader != nullptr);
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to get ArchetypeChunkHeader for adding component to entity ");
						//TODO : Call engine shutdown and error handling here as this is a serious error

						//code excecution stops here
				}

				//call method to do data transfer and get back EntityRecordUpdate,

				EntityRecordUpdate entityRecordUpdate = addInitialComponentDataToArchetypeChunk(archetypeChunkHeader, entityAddInfo);

				//This may seem like unnecssary work to add a single EntityRecordUpdate to a vector and return it
				//But its to facilitate the retrun of next condition where we move between archetypeChunks if it were to occur where there might be more than one EntityRecordUpdate
				std::vector<EntityRecordUpdate>  entityRecordUpdateList;
				entityRecordUpdateList.push_back(entityRecordUpdate);

				//Return EntityRecordUpdateList
				return entityRecordUpdateList;


			}
			else //2. Moving from old archetype to new archetype
			{


				//get old archetypeSignature from entityAddInfo and OR it to 
				// newEntityArchetypeSignature to get the newest Destination ArchetypeSignature
				newEntityArchetypeSignature |= entityAddInfo.entityRecord.archetypeChunkHeader->archetypeDefinition->archetypeSignature;

				//get source and destination chunks
				ArchetypeChunkHeader* srcArchetypeChunk = entityAddInfo.entityRecord.archetypeChunkHeader;
				ArchetypeChunkHeader* dstArchetypeChunk = createOrGetArchetypeChunk(newEntityArchetypeSignature);

				if (dstArchetypeChunk == nullptr)
				{
					assert(dstArchetypeChunk != nullptr);
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to get destination ArchetypeChunk for moving entity ");
					//TODO : Call engine shutdown and error handling here as this is a serious error

					//code excecution stops here
				}

				//call method to do move and get record update back
				//This is upshift and never downshift, so no need to remove any component data
				std::vector<EntityRecordUpdate> entityRecordUpdateList = transferAndAddEntityDataToArchetypeChunk(dstArchetypeChunk, srcArchetypeChunk, entityAddInfo);


				return entityRecordUpdateList;
			}











		}


		const std::unordered_map<ArchetypeSignature, std::unique_ptr<ArchetypeDefinition>>& ArchetypeManager::getArchetypeDefinitions() const
		{
			return m_archetypeDefinitions;
		}





		std::vector<ArchetypeChunkHeader*>  ArchetypeManager::getArchetypeChunksWithEntityData(ArchetypeSignature archetypeSignature)
		{
			

			ChunkList& chunkList = m_archetypeChunksMap.at(archetypeSignature);
			std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders(chunkList.fullChunks);

			if (chunkList.availableChunks.size() > 0 && chunkList.availableChunks.front()->chunkEntityUsed > 0)archetypeChunkHeaders.push_back(chunkList.availableChunks.front());

	
		
			return archetypeChunkHeaders;
		}




		
	

}



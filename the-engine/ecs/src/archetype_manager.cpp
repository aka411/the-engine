#include "archetype_manager.h"
#include <vector>
#include <memory>

namespace TheEngine::ECS
{
	//Aim to write less code and more readable code with proper error handling and performance in mind



	//Error Handling : We are gonna do strict error handling in this class as any error here can cause silent data corruption,
	//                 any unexpected beahavior should be handled by shutting down the engine and logging the error


	//ToDo : the src and dest convention in method signature of this class method 
	// calls and the move function in component is not consistent so need to correct it or carefully check it

	//TODO : The move to available list and move to full list have a lot of code duplication so need to refactor it to avoid code duplication

	//TODO : ADD Assertions in all methods to check for preconditions and postconditions, only for debug builds



	bool ArchetypeManager::moveArchetypeChunkHeaderToAvailableList(ArchetypeChunkHeader* archetypeChunkHeader)
	{
		//ToDo : Review move to available list logic 

		if (archetypeChunkHeader == nullptr)
		{
			//if this happens something went wrong somewhere before this call meaning some code failed to handle error properly
			//ToDo : Write better message
			assert(archetypeChunkheader != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is nullptr in moveArchetypeChunkHeaderToAvailableList")

				//TODO : Call engine shutdown and error handling here as this is a serious error

				return false;//code excecution should never reach here
		}


		const ArchetypeSignature& signature = archetypeChunkHeader->archetypeDefinition->signature;
		//TODO : add null check



		auto it = m_archetypeChunksMap.find(signature);

		if (it != m_archetypeChunksMap.end())
		{
			//we have found the ChunkList for this signature

			ChunkList& chunkList = it->second;

			//for convenience we make references to the vectors in chunkList, this is just to make code more readable
			std::vector<ArchetypeChunkHeader*>& fullChunks = chunkList.fullChunks;
			std::vector<ArchetypeChunkHeader*>& availableChunks = chunkList.availableChunks;


			//we need to find the archetypeChunk Header pointer to be moved in full list and move it to available list
			for (size_t i = 0; i < fullChunks.size(); ++i)
			{
				if (fullChunks[i] == archetypeChunkHeader)
				{
					//Found the chunk header in full list, move it to available list
					availableChunks.push_back(archetypeChunkHeader);

					//overwrite the the copied pointer with last element and pop back to avoid gaps in vector
					fullChunks[i] = fullChunks.back();
					fullChunks.pop_back();


					return true;
				}
			}



		}
		else
		{
			//No signature found, this means the archetype chunk is not managed by this manager and its a leak

			//The best way to handle this is shutdown cause this should never happen , all resources should have been properly managed

			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeSignature not found in moveArchetypeChunkHeaderToAvailableList")

				//TODO : Call engine shutdown and error handling here as this is a serious error

				return false;//code excecution should never reach here
		}

		//If we reach here, something went wrong
		return false;//code excecution should never reach here
	}

	bool ArchetypeManager::moveArchetypeChunkHeaderToFullList(ArchetypeChunkHeader* archetypeChunkHeader)
	{





		if (archetypeChunkHeader == nullptr)
		{
			//if this happens something went wrong somewhere before this call meaning some code failed to handle error properly
			//ToDo : Write better message
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is nullptr in moveArchetypeChunkHeaderToAvailableList")

				//TODO : Call engine shutdown and error handling here as this is a serious error

				return false;//code excecution should never reach here
		}


		const ArchetypeSignature& signature = archetypeChunkHeader->archetypeDefinition->signature;

		auto it = m_archetypeChunksMap.find(signature);

		if (it != m_archetypeChunksMap.end())
		{
			ChunkList& chunkList = it->second;

			std::vector<ArchetypeChunkHeader*>& fullChunks = chunkList.fullChunks;
			std::vector<ArchetypeChunkHeader*>& availableChunks = chunkList.availableChunks;



			for (size_t i = 0; i < availableChunks.size(); ++i)
			{
				if (availableChunks[i] == archetypeChunkHeader)
				{
					//Found the chunk header in full list, move it to available list
					fullChunks.push_back(archetypeChunkHeader);
					//Remove from full list
					//And to avoid gaps in vector, replace with last element and pop back
					availableChunks[i] = availableChunks.back();
					availableChunks.pop_back();
					return true;
				}
			}



		}
		else
		{
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeSignature not found in moveArchetypeChunkHeaderToAvailableList")

				//TODO : Call engine shutdown and error handling here as this is a serious error
				return false;//ToDo : change to proper error handling as this is gonna cause a silent corruption
		}

		//If we reach here, something went wrong
		return false;

	}





	bool ArchetypeManager::moveEntityData(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityRecord* entityRecord, std::vector<EntityRecordUpdate>& entityRecordUpdateList)
	{

		//Aim : To move data of entity from 

		if (srcArchetypeChunkHeader == nullptr || destArchetypeChunkHeader == nullptr || entityRecord == nullptr)
		{
			//some thing went wrong somewhere before this call meaning some code failed to handle error properly

			if (srcArchetypeChunkHeader == nullptr)
			{
				LOG_ERROR(m_logger, LogSource::ECS, "Source ArchetypeChunkHeader is null in moveEntityData")
			}
			if (destArchetypeChunkHeader == nullptr)
			{
				LOG_ERROR(m_logger, LogSource::ECS, "Destination ArchetypeChunkHeader is null in moveEntityData")
			}
			if (entityRecord == nullptr)
			{
				LOG_ERROR(m_logger, LogSource::ECS, "EntityRecord is null in moveEntityData")
			}

			//TODO : Call engine shutdown and error handling here as this is a serious error

			return false;// code excecution should never reach here
		}


		//ToDo : Review and  move logic and gap filling logic
		//ToDo : check all pointer arthemetic and logic

		//ToDo : Carefully go through code and add checks and error handling where needed


		//Aim : Transfer exsisting data , delete left data  ,send out commands to update both records to entity manager
		//Notes : After this method call Entity Record is invalid and should not be used anymore untill updated by entity manager which will only happen after external call to entity manager with the returned EntityRecordUpdate

		const ArchetypeDefinition* srcArchetypeDefinition = srcArchetypeChunkHeader->archetypeDefinition;
		const ArchetypeDefinition* destArchetypeDefinition = destArchetypeChunkHeader->archetypeDefinition;


		const std::vector<ComponentLayout>& srcComponentLayouts = srcArchetypeDefinition->componentLayouts;
		const std::vector<ComponentLayout>& destComponentLayouts = destArchetypeDefinition->componentLayouts;


		const int numSrcComponents = srcComponentLayouts.size();
		const int numDestComponents = destComponentLayouts.size();


		ArchetypeChunk* srcArchetypeChunk = srcArchetypeChunkHeader->archetypeChunk;
		ArchetypeChunk* destArchetypeChunk = destArchetypeChunkHeader->archetypeChunk;


		ArchetypeRecordChunk* srcArchetypeRecordChunk = srcArchetypeChunkHeader->archetypeRecordChunk;
		ArchetypeRecordChunk* destArchetypeRecordChunk = destArchetypeChunkHeader->archetypeRecordChunk;


		size_t srcChunkBasePtr = reinterpret_cast<size_t>(srcArchetypeChunk->data);
		size_t destChunkBasePtr = reinterpret_cast<size_t>(destArchetypeChunk->data);

		//Note : using pointers to get data in loop is gonna cause cpu cache misses so reduce the number of pointer dereferences in the loop

		//ToDo : add check if destFreeIndex is within bounds of dest chunk capacity for safety, but should never happen if logic is correct

		//move data from src to dest based on component types in both archetypes

		const ArchetypeSignature& srcArchetypeSignature = srcArchetypeDefinition->signature;
		for (auto& destComponentLayout : destComponentLayouts)
		{


			if (srcArchetypeSignature.test(destComponentLayout.componentId))
			{
				size_t srcComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (entityRecord->index * srcComponentLayout.componentTypeInfo.size);
				size_t destComponentPtr = destChunkBasePtr + destComponentLayout.offsetInChunk + (destArchetypeChunkHeader->chunkEntityUsed * destComponentLayout.componentTypeInfo.size);


				//move
				destComponentLayout.componentTypeInfo.moveConstructor(destComponentPtr, srcComponentPtr);


			}


		}

		//Done : loop for destructing goes here

		for (auto& srcComponentLayout : srcComponentLayouts)
		{


			size_t srcComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (entityRecord->index * srcComponentLayout.componentTypeInfo.size);
			//destruct
			srcComponentLayout.componentTypeInfo.destructor(srcComponentPtr);

		}



		//ToDo : update archetype chunk headers accordingly and also archetype records, 
		// 
		//	First take source ArchetypeChunkHeader and reduce src count by one and check if it was the last entity in chunk
		// 
		//		if so , put it in available stop here
		//		if not , we need to fill the gap left by move in src chunk
		//			get last entity record in src chunk
		//			move its data to the gap
		//			destruct last entity data in src chunk if needed
		//			update last entity record to point to new index, update archetype record accordingly
		//

		if (srcArchetypeChunkHeader->chunkEntityUsed != 0)//safety check to avoid underflow, this should never happen if logic is correct
		{
			srcArchetypeChunkHeader->--chunkEntityUsed;
			//WARNING : AFTER THIS LINE srcArchetypeChunkHeader->chunkEntityUsed CANNOT BE USED TO CALCULATE LAST ENTITY INDEX AS IT IS DECREMENTED BY ONE SO NEED TO ADD ONE BACK IF NEEDED
		}
		else
		{
			LOG_ERROR(m_logger, LogSource::ECS, "Source ArchetypeChunkHeader entity used count is already zero in moveEntityData")
				return false;//ToDo : change to proper error handling as this is gonna cause a silent corruption
		}



		//Gap Handling Logic Here


		if (srcArchetypeChunkHeader->chunkEntityUsed != 0)
		{
			//fill gap logic here
			const size_t lastEntityIndex = srcArchetypeChunkHeader->chunkEntityUsed + 1; //as we decremented above


			for (auto& srcComponentLayout : srcComponentLayouts)
			{


				size_t srcComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (lastEntityIndex * srcComponentLayout.componentTypeInfo.size);
				size_t destComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (entityRecord->index * srcComponentLayout.componentTypeInfo.size);

				//move
				srcComponentLayout.componentTypeInfo.moveConstructor(destComponentPtr, srcComponentPtr);

				//destruct
				srcComponentLayout.componentTypeInfo.destructor(srcComponentPtr);

			}

			// update last entity record to point to new index, update archetype record accordingly

			srcArchetypeRecordChunk.id[entityRecord->index] = srcArchetypeRecordChunk.id[lastEntityIndex];

			EntityRecordUpdate lastEntityRecordUpdate;
			lastEntityRecordUpdate.entityId = srcArchetypeRecordChunk.id[lastEntityIndex];
			lastEntityRecordUpdate.newArchetypeChunkHeader = srcArchetypeChunkHeader;
			lastEntityRecordUpdate.newIndex = entityRecord->index;

			entityRecordUpdateList.push_back(lastEntityRecordUpdate);
		}
		else
		{
			//move to available list logic here, but it should already be empty so nothing to do, but there is a chance that this chunk could only hold one entity
			//ToDo : implement move to available list logic
			bool isMoved = moveArchetypeChunkHeaderToAvailableList(srcArchetypeChunkHeader);

		}


		//Now handle destination chunk header

		const size_t destIndex = destArchetypeChunkHeader->chunkEntityUsed;

		destArchetypeChunkHeader->chunkEntityUsed++;


		destArchetypeRecord->id[destIndex] = entityRecord->id;


		if (destArchetypeChunkHeader->chunkEntityUsed == destArchetypeChunkHeader->ArchetypeChunkRecord.maxCount)
		{
			//move to full list logic here
			//ToDo : implement move to full list logic
			bool isMoved = moveArchetypeChunkHeaderToFullList(destArchetypeChunkHeader);

		}



		EntityRecordUpdate destEntityRecordUpdate;

		destEntityRecordUpdate.entityId = entityRecord->id;
		destEntityRecordUpdate.newArchetypeChunkHeader = destArchetypeChunkHeader;
		destEntityRecordUpdate.newIndex = destIndex;

		entityRecordUpdateList.push_back(destEntityRecordUpdate);



		return true;


	}

	std::vector<EntityRecordUpdate> ArchetypeManager::transferEntityBetweenArchetype(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityAddInfo entityAddInfo)
	{


		//Aim : To transfer data  between srcArchetypeChunkHeader and destArchetypeChunkHeader based on entityAddInfo
		//transfer exsisting data , delete left data or add new data , update both records ;

		std::vector<EntityRecordUpdate> entityRecordUpdateList;

		bool moveResult = moveEntityData(srcArchetypeChunkHeader, destArchetypeChunkHeader, &entityAddInfo.entityRecord, entityRecordUpdateList);




		const std::vector<ComponentLayout>& destComponentLayouts = destArchetypeChunkHeader->archetypeDefinition->componentLayouts;
		ArchetypeChunk* destArchetypeChunk = destArchetypeChunkHeader->archetypeChunk;
		size_t destChunkBasePtr = reinterpret_cast<size_t>(destArchetypeChunk->data);
		//Note : No need to update any records here as moveEntityData already did that
		for (EntityRecordUpdate& entityRecordUpdate : entityRecordUpdateList)
		{
			if (entityRecord.id == entityAddInfo.id)
			{

				const size_t destIndex = entityRecord.newIndex;

				//ToDo : add rest of component data from entityAddInfo to destArchetypeChunkHeader at destIndex

				for (ComponentLayout& destComponentLayout : destComponentLayouts)
				{
					for (ComponentData& componentData : entityAddInfo.componentDataList)
					{
						if (destComponentLayout.componentId == componentData.componentId)
						{
							size_t destComponentPtr = destChunkBasePtr + destComponentLayout.offsetInChunk + (destIndex * destComponentLayout.componentTypeInfo.size);
							//move construct
							destComponentLayout.componentTypeInfo.moveConstructor(static_cast<void*>(destComponentPtr), componentData.data);

							//call destructor on source data 
							//Note : Actually not needed as we moved data from src chunk in moveEntityData so no source data exsists for these new components
							//Also all these src data is in temp memory block which will be freed by caller
							destComponentLayout.componentTypeInfo.destructor(componentData.data);

							break; //break inner loop as we found the component
						}
					}

				}
				break; //break outer loop as we found the entity

			}

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
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is null in addComponentDataToArchetypeChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE

				return EntityRecordUpdate{};
		}


		
		ArachetypeChunk* archetypeChunk = archetypeChunkHeader->archetypeChunk;
		if (archetypeChunk != nullptr)
		{
			assert(archetypeChunk != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunk is nullptr in addComponentDataToArchetypeChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE

				return EntityRecordUpdate{};
		}


		ArchetypeRecordChunk* archetypeRecordChunk = archetypeChunkHeader->archetypeRecordChunk;
		if (archetypeRecordChunk != nullptr)
		{
			assert(archetypeRecordChunk != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeRecordChunk is nullptr in addComponentDataToArchetypeChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE

				return EntityRecordUpdate{};
		}


		const ArchetypeDefinition* archetypeDefinition = archetypeChunkHeader->archetypeDefinition;
		if (archetypeDefinition != nullptr)
		{
			assert(archetypeDefinition != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeDefinition is nullptr in addComponentDataToArchetypeChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE

				return EntityRecordUpdate{};
		}


		const std::vector<ComponentLayout>& destComponentLayouts = archetypeDefinition->componentLayouts;//from archetypehCunkHeader
		//TODO : do i need to check for zero size of destComponentLayouts ,most probaly not


		size_t archetypeChunkBaseAddress = reinterpret_cast<size_t>(archetypeChunk->data);
		const size_t destIndex = archetypeChunkHeader->chunkEntityUsed;

		std::vector<ComponentData>& componentDataList = entityAddInfo.componentDataList;

		// To check if the data transfer is within bounds of ArchetypeChunk  for safety, but should never happen if logic is correct
		const size_t archetypeChunkRawSize = archetypeDefinition->chunkRawSize;
		const size_t archetypeChunkEndAddress = archetypeChunkBaseAddress + archetypeChunkRawSize;


		for (ComponentData& srcComponentData : componentDataList) 
		{
			for (ComponentLayout& destComponentLayout : destComponentLayouts)
			{
				if (srcComponentData.componentId == destComponentLayout.componentId)
				{
					//TODO : need to review this
					size_t destComponentAddress = archetypeChunkBaseAddress + destComponentLayout.offsetInChunk + (destIndex * destComponentLayout.componentTypeInfo.size);

					

					if ((archetypeChunkEndAddress) < (destComponentAddress + destComponentLayout.componentTypeInfo.size)) // safety check
					{
						assert((archetypeChunkEndAddress) < (destComponentAddress + destComponentLayout.componentTypeInfo.size));
						LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunk OVERFLOW detected during component data addition. ")
							//TODO : Call engine shutdown and error handling here as this is a serious error
							//CODE EXECUTION STOPS HERE

							return EntityRecordUpdate{};
					}


					//move construct
					destComponentLayout.componentTypeInfo.moveConstructor(static_cast<void*>(destComponentAddress), srcComponentData.data);

					// call destructor on source data for safety
					//what if there is a bad move constructor, wont that resource be lost?
					//Note : Actually not needed as we moved data from src chunk in moveEntityData so no source data exsists for these new components
					//Also all these src data is in temp memory block which will be freed by caller

					destComponentLayout.componentTypeInfo.destructor(srcComponentData.data);
					break; //break inner loop as we found the component and find next in source
				}
			}
		}

		//Update Records in ArchetypeChunk Header and its relevant records

		archetypeChunkHeader->chunkEntityUsed++;
		//check if full and if full move to full chunk list

		destArchetypeRecordChunk.id[destIndex] = entityAddInfo.entityRecord.id;

		//TODO : The maxcount in ArchetypeChunkRecord might get moved to header itself so be wary of design change
		if (archetypeChunkHeader->chunkEntityUsed == archetypeChunkHeader->ArchetypeChunkRecord.maxCount)
		{
			//move to full list logic here
	
			bool isMoved = moveArchetypeChunkHeaderToFullList(archetypeChunkHeader);

			assert(isMoved);
			LOG_ERROR(m_logger, LogSource::ECS, "Failed to move ArchetypeChunkHeader to fullList in ChunkList ")
				//TODO : Call engine shutdown and error handling here as this is a serious error 
				//CODE EXECUTION STOPS HERE

		}

		EntityRecordUpdate entityRecordUpdate;

		entityRecordUpdate.entityId = entityAddInfo.entityRecord.id;
		entityRecordUpdate.newArchetypeChunkHeader = archetypeChunkHeader;
		entityRecordUpdate.newIndex = destIndex;

		return entityRecordUpdate;


	}

	ArchetypeDefinition* ArchetypeManager::createNewArchetypeDefinition(const ArchetypeSignature& archetypeSignature)
	{
		//REVIEW : In progress(90 percent Complete)

		//Aim : To create new ArchetypeDefinition based on given ArchetypeSignature
		
		//This method interacts with ComponentManager to get component type info for each component in the signature
		std::vector<ComponenTypeinfo> componentTypeInfos;

		//ToDo : The below loop is very inefficient as it loops  MAX_COMPONENTS times, a better way would be to find the
		//       first set bit and then next set bit and so on Or find first set bit and proceed from there

		// Method A-1 vvvvvvvv: Loops through whole bits  
		for (ComponentId componentId = 0; componentId < MAX_COMPONENTS; ++componentId)
		{
			if (archetypeSignature.test(componentId))
			{
				

				ComponentTypeInfo* componentTypeInfo = m_componentRegistery.getComponentTypeInfo(componentId);

				if (componentTypeInfo == nullptr)
				{
					assert(componentTypeInfo != nullptr);
					LOG_ERROR(m_logger, LogSource::ECS, "ComponentTypeInfo not found for componentId in createNewArchetypeDefinition")
                    //TODO : Call engine shutdown and error handling here as this is a serious error
					//CODE EXECUTION STOPS HERE

					return nullptr;
				}

				componentTypeInfos.push_back(*componentTypeInfo);


			}
		}
		// Method A-1 ^^^^^^^^^^^: Loops through whole bits  
		 
	
		// Method A-2 vvvvvv: First get MSB then loop from there

		//Research into this has lead me to the conclusion that a single cycle method to find MSB does not exsist for std::bitset
		// Method A-2 ^^^^^^^: First get MSB then loop from there



		//we need to sort componentTypeInfos by alignment then size for proper memory layout
		//in Descending order of alignment 
		std::sort(componentTypeInfos.begin(), componentTypeInfos.end(), [](const ComponentTypeInfo& a, const ComponentTypeInfo& b)
			{
				if (a.alignment == b.alignment)
				{
					return a.size > b.size; //larger size first if same alignment
				}
				return a.alignment > b.alignment; //larger alignment first
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
			totalComponentsSize += componentTypeInfos[i].size;
		}

		size_t requiredChunkSize = totalComponentsSize * MAX_NUM_OF_ENTITIES_PER_CHUNK;


		//create component layouts with offsets
		size_t currentOffset = 0;

		for (size_t i = 0; i < componentTypeInfos.size(); ++i)
		{
			ComponentLayout layout;
			layout.componentId = componentTypeInfos[i].componentId;
			layout.size = componentTypeInfos[i].size;
			layout.alignment = componentTypeInfos[i].alignment;
			layout.offsetInChunk = currentOffset;
			layout.componentTypeInfo = &componentTypeInfos[i];

			currentOffset += componentTypeInfos[i].size * MAX_NUM_OF_ENTITIES_PER_CHUNK; //increment offset by size * max entities to get next component offset

			componentLayouts.push_back(layout);
		}

		size_t totalChunkSize = MAX_NUM_OF_ENTITIES_PER_CHUNK * totalComponentsSize;//in bytes


		std::unique_ptr<ArchetypeDefinition> newArchetypeDefinition = std::make_unique<ArchetypeDefinition>();

		newArchetypeDefinition->archetypeSignature = archetypeSignature;
		newArchetypeDefinition->componentLayouts = componentLayouts;
		newArchetypeDefinition->chunkRawSize = totalChunkSize;


		//store in map

		m_archetypeDefinitions[archetypeSignature] = std::move(newArchetypeDefinition);

		// get newly created and stored ArchetypeDefinition and returns a pointer to it

		auto& it = m_archetypeDefinitions.find(archetypeSignature);

		if (it != m_archetypeDefinitions.end())
		{
			return it->second.get();//pointer to Archetype Definition
		}
		else
		{
			assert((it != m_archetypeDefinitions.end());
			LOG_ERROR(m_logger, LogSource::ECS, "Newly created ArchetypeDefinition not found in ArchetypeSignature-ArchetypeDefinition Map")
				//TODO : Call engine shutdown and error handling here as this is a serious error

			return nullptr;//code execution should not reach here, if it reaches here that means engine shutdown failed
		}




		return nullptr;//code execution should not reach here
	}

	ArchetypeChunkHeader* ArchetypeManager::createArchetypeChunkHeader(ArchetypeDefinition* archetypeDefinition)
	{
		//REVIEW : Done
		//Aim : To allocate a new ArchetypeChunkHeader along with ArchetypeChunk and ArchetypeRecordChunk from ArchetypeDefinition


		void* archetypeChunkHeaderRawPtr = std::aligned_alloc(alignof(ArchetypeChunkHeader), sizeof(ArchetypeChunkHeader));
		if (archetypeChunkHeader == nullptr)
		{
			assert(archetypeChunkHeader != nullptr)
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE
		}

		//TODO : Repalce with correct alignment requirement
		void* archetypeChunkRawPtr = std::aligned_alloc(alignof(/*replace with first component alignment requirement*/), /*replace with chunk size*/);
		
		if (archetypeChunkRawPtr == nullptr)
		{
			std::free(archetypeChunkHeader);//TODO : these are fragile need better way to do this 
			assert(archetypeChunkRawPtr != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
	            //CODE EXECUTION STOPS HERE
			
		}

		void* ArchetypeRecordChunkRawPtr = std::aligned_alloc(alignof(ArchetypeRecordChunk), /*replace with chunk size*/);
		
		if (ArchetypeRecordChunkRawPtr == nullptr)
		{
			std::free(archetypeChunkHeader);
			std::free(archetypeChunkRawPtr);
			assert(ArchetypeRecordChunkRawPtr != nullptr);
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeRecordChunk")
				//TODO : Call engine shutdown and error handling here as this is a serious error
				//CODE EXECUTION STOPS HERE
			
		}


		//All allocations were successful

		//casting them to correct type
		ArchetypeChunkHeader* newArchetypeChunkHeader = static_cast<ArchetypeChunkHeader*>(archetypeChunkHeaderRawPtr);
		ArchetypeChunk* newArchetypeChunk = static_cast<ArchetypeChunk*>(archetypeChunkRawPtr);
		ArchetypeRecordChunk* newArchetypeRecordChunk = static_cast<ArchetypeRecordChunk*>(ArchetypeRecordChunkRawPtr);


	
		newArchetypeChunkHeader->archetypeDefinition = archetypeDefinition;

		//Linking them ,think of it as a binary tree with ArchetypeChunkHeader as parent and ArchetypeChunk and ArchetypeRecord as two child of it.
		newArchetypeChunkHeader->archetypeChunk = newArchetypeChunk;
		newArchetypeChunkHeader->archetypeRecord = newArchetypeRecordChunk;

		//we only need to return it , storing will be handled by caller
		return newArchetypeChunkHeader;
	}




	ArchetypeChunk* ArchetypeManager::createOrGetArchetypeChunk(ArchetypeSignature archetypeSignature)
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
					assert(archetypeDef != nullptr)
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to get archetypeDef , there is corruption somewhere or need to review logic in Chunklist and archetype lifetime relations ")
						//TODO : Call engine shutdown and error handling here as this is a serious error

						//code excecution stops here cause if there is 
						//exisiting ChunkList for a ArchetypeSignature then there must be a ArchetypeDefinition for it
						return nullptr;
				}


				// create new ArchetypeChunkHeader

				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(archetypeDef);

				if (newArchetypeChunkHeader == nullptr)
				{
					assert(newArchetypeChunkHeader != nullptr)
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader")
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
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader")
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						reteun nullptr;
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
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeDefinition for signature")
						//TODO : Call engine shutdown and error handling here as this is a serious error
						//CODE EXECUTION STOPS HERE
						return nullptr;
				}





				//Create new ArchetypeChunk
				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(newArchetypeDefinition);

				if (newArchetypeChunkHeader == nullptr)
				{
					assert(newArchetypeChunkHeader != nullptr)
						LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader")
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


		ArchetypeManager::ArchetypeManager()
		{
			//ToDo : Implement Constructor
		}

		ArchetypeManager::~ArchetypeManager()
		{
			//ToDo : Implement Destructor
			//very fragile code , review carefully
			for (auto& pair : m_archetypeChunksMap)
			{
				ChunkList& chunkList = pair.second;
				for (ArchetypeChunkHeader* chunkHeader : chunkList.fullChunks)
				{
					std::free(chunkHeader->archetypeChunk);
					std::free(chunkHeader->archetypeRecord);
					std::free(chunkHeader);
				}

				for (ArchetypeChunkHeader* chunkHeader : chunkList.availableChunks)
				{
					std::free(chunkHeader->archetypeChunk);
					std::free(chunkHeader->archetypeRecord);
					std::free(chunkHeader);
				}
			}





		}




		std::vector<EntityRecordUpdate> ArchetypeManager::addComponentToEntity(EntityAddInfo entityAddInfo)
		{
			//  REVIEW : Done

			ArchetypeSignature newEntityArchetypeSignature;

			//Add up all component ids and make up the new ArchetypeSignature 
			for (auto& componentData : entityAddInfo.componentDataList)
			{
				newEntityArchetypeSignature.set(componentData.componentId);
			}



			//The next stpe can go in one of two ways
			 //1. Initial add of data to archetype
			 //2. Moving from old archetype to new archetype

			if (entityAddInfo.archetypeChunk == nullptr) // 1. Initial add of data to archetype
			{



				//get a archetypeChunkHeader with atleast one slot free for adding data
				ArchetypeChunkHeader* archetypeChunkHeader = createOrGetArchetypeChunk(newEntityArchetypeSignature);

				if (archetypeChunkHeader == nullptr)
				{
					assert(archetypeChunkHeader != nullptr)
						LOG_ERROR(m_logger, LogSource::ECS, "Failed to get ArchetypeChunkHeader for adding component to entity ")
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
				newEntityArchetypeSignature |= entityAddInfo.archetypeChunkHeader->archetypeDefinition->signature;

				//get source and destination chunks
				ArchetypeChunkHeader* srcArchetypeChunk = entityAddInfo.archetypeChunkHeader;
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
				std::vector<EntityRecordUpdate> entityRecordUpdateList = transferEntityBetweenArchetype(srcArchetypeChunk, dstArchetypeChunk, entityAddInfo);


				return entityRecordUpdateList;
			}











		}





	}

}

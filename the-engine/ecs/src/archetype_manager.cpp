#include "archetype_manager.h"
#include <vector>

namespace TheEngine::ECS
{







	bool ArchetypeManager::moveEntityData(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityRecord* entityRecord, EntityRecordUpdate* entityRecordUpdate)
	{
		//ToDo : add nullptr checks
		//ToDO : check all pointer arthemetic and logic
		//ToDo : Implement data move between srcArchetypeChunkHeader and destArchetypeChunkHeader based on entityRecord



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
		/*
		// Method 1 : nested loop through dest components and src components to find matching component types and move data accordingly
		for (auto& destComponentLayout : destComponentLayouts)
		{
			for (auto& srcComponentLayout : srcComponentLayouts)
			{
				if (destComponentLayout.componentId == srcComponentLayout.componentId)
				{


					size_t srcComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (entityRecord->index * srcComponentLayout.componentTypeInfo.size);
					size_t destComponentPtr = destChunkBasePtr + destComponentLayout.offsetInChunk + (destArchetypeChunkHeader->chunkEntityUsed * destComponentLayout.componentTypeInfo.size);


					//move
					destComponentLayout.componentTypeInfo.moveConstructor(destComponentPtr, srcComponentPtr);

					//destruct
					srcComponentLayout.componentTypeInfo.destructor(srcComponentPtr);

				}

			}


		}
		*/

		//Method 2 : more optimized loop based on wether src has the component or not
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

		//Note : i think i should run the destructor as separate loop after all moves are done cause if src contains more components than dest then some components will be left without move and need to be destructed

		//Done : loop for destructing goes here
		
		for(auto& srcComponentLayout : srcComponentLayouts)
		{
			
			
				size_t srcComponentPtr = srcChunkBasePtr + srcComponentLayout.offsetInChunk + (entityRecord->index * srcComponentLayout.componentTypeInfo.size);
				//destruct
				srcComponentLayout.componentTypeInfo.destructor(srcComponentPtr);
			
		}



		//ToDo : update archetype chunk headers accordingly and also archetype records, 
		// we need to fill the gap left by move in src chunk

			//increment dest chunk entity used count
			//decrement src chunk entity used count



		//send out dest entity record update with new chunk and index
		//send out src entity record to remove entity reference


		//if src chunk is empty after move , mark it as available , entityre
		//if dest chunk is full after move , mark it as full



		//return true if successful , false otherwise



		const size_t srcEntityIndex = entityRecord->index;

		const size_t destFreeIndex = destArchetypeChunkHeader->chunkEntityUsed;

		const size_t destEntityIndex = destFreeIndex;







		return false;
	}

	std::vector<EntityRecordUpdate> ArchetypeManager::transferEntityBetweenArchetype(ArchetypeChunkHeader* srcArchetypeChunkHeader, ArchetypeChunkHeader* destArchetypeChunkHeader, EntityAddInfo entityAddInfo)
	{


		//ToDo : Implement data transfer between srcArchetypeChunkHeader and destArchetypeChunkHeader based on entityAddInfo
		//transfer exsisting data , delete left data or add new data , update both records ;






		return std::vector<EntityRecordUpdate>();
	}

	EntityRecordUpdate ArchetypeManager::addComponentDataToArchetypeChunk(ArchetypeChunkHeader* archetypeChunkHeader, EntityAddInfo entityAddInfo)
	{

		if (archetypeChunkHeader == nullptr)
		{
			LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeChunkHeader is null in addComponentDataToArchetypeChunk")
				return EntityRecordUpdate{};//ToDo : change to proper error handling as this is gonna cause a silent corruption
		}


		//ToDo : Implement data transfer from entityAddInfo to archetypeChunkHeader->archetypeChunk and update archetypeChunkHeader->archetypeRecord accordingly

	}

	ArchetypeChunkHeader* ArchetypeManager::createArchetypeChunkHeader(ArchetypeDefinition* archetypeDefinition)
	{

		void* archetypeChunkHeaderRawPtr = std::aligned_alloc(alignof(ArchetypeChunkHeader), sizeof(ArchetypeChunkHeader));
		if (archetypeChunkHeader == nullptr)
		{
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader")
				return nullptr;
		}

		void* archetypeChunkRawPtr = std::aligned_alloc(alignof(/*replace with first component alignment requirement*/), /*replace with chunk size*/);
		if (archetypeChunkRawPtr == nullptr)
		{
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunk")
				std::free(archetypeChunkHeader);
			return nullptr;
		}

		void* ArchetypeRecordChunkRawPtr = std::aligned_alloc(alignof(ArchetypeRecordChunk), /*replace with chunk size*/);
		if (ArchetypeRecordChunkRawPtr == nullptr)
		{
			LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeRecordChunk")
				std::free(archetypeChunkHeader);
			std::free(archetypeChunkRawPtr);
			return nullptr;
		}

		ArchetypeChunkHeader* newArchetypeChunkHeader = static_cast<ArchetypeChunkHeader*>(archetypeChunkHeaderRawPtr);
		ArchetypeChunk* newArchetypeChunk = static_cast<ArchetypeChunk*>(archetypeChunkRawPtr);
		ArchetypeRecordChunk* newArchetypeRecordChunk = static_cast<ArchetypeRecordChunk*>(ArchetypeRecordChunkRawPtr);

		newArchetypeChunkHeader->archetypeDefinition = archetypeDefinition;
		newArchetypeChunkHeader->archetypeChunk = newArchetypeChunk;
		newArchetypeChunkHeader->archetypeRecord = newArchetypeRecordChunk;


		return newArchetypeChunkHeader;
	}




	ArchetypeChunk* ArchetypeManager::createOrGetArchetypeChunk(ArchetypeSignature archetypeSignature)
	{

		//try to find existing ChunkList for this archetypeSignature
		auto it = m_archetypeChunksMap.find(archetypeSignature);




		if (it != m_archetypeChunksMap.end())
		{
			//Found existing ChunkList for this archetypeSignature


			ChunkList& chunkList = it->second;
			//Check for available chunks
			if (!chunkList.availableChunks.empty())
			{
				//Return the first available chunk
				return chunkList.availableChunks.back();
			}
			else
			{
				//No available chunks, need to create a new one
				ArchetypeDefinition* archetypeDef = m_archetypeDefinitions[archetypeSignature].get();
				if (archetypeDef == nullptr)
				{
					LOG_ERROR(m_logger, LogSource::ECS, "ArchetypeDefinition not found for signature")
						return nullptr;
				}



				// create new ArchetypeChunkHeader


				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(archetypeDef);
				if (newArchetypeChunkHeader == nullptr)
				{
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader")
						return nullptr;
				}

				chunkList.availableChunks.push_back(newArchetypeChunkHeader);

				return newArchetypeChunkHeader;
			}
		}
		else
		{
			// Did not find a already exsisting archetypeSignature



			//check if definition exsists
			auto defIt = m_archetypeDefinitions.find(archetypeSignature);



			if (defIt != m_archetypeDefinitions.end())
			{

				//create new ChunkList from definition
				ChunkList newChunkList;

				ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(defIt->second.get());

				if (newArchetypeChunkHeader == nullptr)
				{
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeChunkHeader")
						return nullptr;
				}

				newChunkList.availableChunks.push_back(newArchetypeChunkHeader);

				m_archetypeChunksMap[archetypeSignature] = newChunkList;

				return newArchetypeChunkHeader;

			}







			//Definition does not exsist, need to create one

			ArchetypeDefinition* newArchetypeDefinition = createNewArchetypeDefinition(archetypeSignature);//ToDo : add method and implementation,dont forget to add to map


			if (newArchetypeDefinition == nullptr)
			{
				LOG_ERROR(m_logger, LogSource::ECS, "Failed to create new ArchetypeDefinition for signature")
					return nullptr;
			}





			//Create new ArchetypeChunk
			ArchetypeChunkHeader* newArchetypeChunkHeader = createArchetypeChunkHeader(newArchetypeDefinition);

			if (newArchetypeChunkHeader == nullptr)
			{
				LOG_ERROR(m_logger, LogSource::ECS, "Memory allocation failed for new ArchetypeChunkHeader")
					return nullptr;
			}




			//Create new ChunkList
			ChunkList newChunkList;


			newChunkList.availableChunks.push_back(newArchetypeChunkHeader);

			//add to map
			m_archetypeChunksMap[archetypeSignature] = newChunkList;


			return newArchetypeChunkHeader;


		}






		/****Public Methods ****/


		ArchetypeManager::ArchetypeManager()
		{
			//ToDo : Implement Constructor
		}

		ArchetypeManager::~ArchetypeManager()
		{
			//ToDo : Implement Destructor

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

			ArchetypeSignature newEntityArchetypeSignature;
			//ToDo : get current archetypeSignature from entityAddInfo and set it to entityArchetypeSignature
			for (auto& componentData : entityAddInfo.componentDataList)
			{
				newEntityArchetypeSignature.set(componentData.componentId);
			}







			//Either a move or total new add

			if (entityAddInfo.archetypeChunk == nullptr)
			{
				//Add Condition

				//get archetypeSignature from entityAddInfo



				//create OR get archetype chunk
				ArchetypeChunkHeader* archetypeChunk = createOrGetArchetypeChunk(newEntityArchetypeSignature);//ToDo : add method and implementation

				if (archetypeChunk == nullptr)
				{
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to get ArchetypeChunk for adding component to entity ")
				}

				//call method to do data transfer and get back EntityRecordUpdate,

				EntityRecordUpdate entityRecordUpdate = addComponentDataToArchetypeChunk(archetypeChunk, entityAddInfo);
				std::vector<EntityRecordUpdate>  entityRecordUpdateList;

				entityRecordUpdateList.push_back(entityRecordUpdate);
				//Return EntityRecordUpdate

				return entityRecordUpdateList;


			}
			else
			{

				//Move Condition

				//get current archetypeSignature from entityAddInfo and add it also to newEntityArchetypeSignature
				newEntityArchetypeSignature |= entityAddInfo.archetypeChunkHeader->archetypeDefinition->signature;

				//get source and destination chunks
				ArchetypeChunkHeader* srcArchetypeChunk = entityAddInfo.archetypeChunkHeader;
				ArchetypeChunkHeader* dstArchetypeChunk = createOrGetArchetypeChunk(newEntityArchetypeSignature);

				if (dstArchetypeChunk == nullptr)
				{
					LOG_ERROR(m_logger, LogSource::ECS, "Failed to get destination ArchetypeChunk for moving entity ")
				}

				//call method to do move and get record update back
				//This is upshift and never downshift, so no need to remove any component data
				std::vector<EntityRecordUpdate> entityRecordUpdateList = transferEntityBetweenArchetype(srcArchetypeChunk, dstArchetypeChunk, entityAddInfo);//ToDo : add method and implementation

				//Return EntityRecordUpdate
				return entityRecordUpdateList;
			}











		}





	}

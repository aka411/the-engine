#include <type_traits>
#include "archetype_manager.h"
#include <vector>


namespace TheEngine::ECS
{


	bool ArchetypeManager::checkFit(const size_t numOfEntities, const size_t chunkRawSize, const size_t archetypeHeaderSize, std::vector<ComponentLayout>& componentLayouts)
	{
		//This function checks if the given number of entities can fit in the chunk of the given size with the given component layouts
		//It will return true if it can fit, else false
			//Potential edge cases to consider :
		//1. If the componentLayouts vector is empty, then it should return true if numOfEntities is 0, else false

		//i should not be modifying the componentLayouts vector its a bad design need to redesign


		size_t currOffset = archetypeHeaderSize;//header size

		for (auto& layout : componentLayouts)
		{
			size_t compAlignment = layout.alignment;
			size_t padding = 0;

			if (currOffset % compAlignment)
			{
				padding = compAlignment - (currOffset % compAlignment);
				currOffset += padding;
			}

			layout.offSet = currOffset;

			currOffset += layout.size() * numOfEntities;

		}


		if (currOffset <= chunkRawSize)
		{
			return true;
		}
		else
		{
			return false;
		}



	}



	ArchetypeChunk* getNewChunk(const ArchetypeDefinition* archetypeDefinition)
	{
		//This function is used to create a new chunk of the given archetypeDefinition
		//It will allocate memory from the custom chunk pool allocator and return a new ArchetypeChunk pointer

		//potential edge cases to consider
			//1.what if memory allocation fails? --> log it and return nullptr and let caller hadle it(not implemented)


		if (archetypeDefinition == nullptr)
		{
			//ToDo: log error and return nullptr
			return nullptr;
		}


		void* newChunk = m_chunkPoolAllocator.allocate(CHUNK_RAW_SIZE, alignof(ArchetypeChunk));


		if (newChunk == nullptr)
		{
			//log error allocation failed for new chunk
			return nullptr; //or halt?
		}

		ArchetypeChunk* archetypeChunk = new(newChunk) ArchetypeChunk();

		archetypeChunk->archetypeDefinition = archetypeDefinition;
		archetypeChunk->chunkRawSize = CHUNK_RAW_SIZE;
		archetypeChunk->chunkEntityCapacity = archetypeDefinition->chunkEntityCapacity;

		archetypeChunk->chunkEntityUsed = 0;

		return archetypeChunk;

	}





	ArchetypeDefinition* ArchetypeManager::getOrCreateArchetypeDefinition(const ArchetypeSignature& signature)
	{
		auto it = m_archetypeDefinitions.find(signature);
		if (it != m_archetypeDefinitions.end())
		{
			return it->second.get();
		}



		/** if not found then create new archetype definition and return it **/

		auto archetypeDefinition = std::make_unique<ArchetypeDefinition>();

		archetypeDefinition->archetypeSignature = signature;


		std::vector<ComponentLayout> componentLayouts;
		for (ComponentId i = 0; i < signature.size(); ++i)
		{

			if (signature[i] == 1)
			{

				ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeinfo(i); //get component type info

				if (componentTypeInfo == nullptr) break; //ToDo : comeup with better handler

				ComponentLayout componentLayout;

				componentLayout.componentId = i;
				componentLayout.size = componentTypeInfo->size;
				componentLayout.alignment = componentTypeInfo->alignment;
				componentLayout.offsetInChunk = 0; //will be set later when chunk is created
				componentLayout.componentTypeInfo = componentTypeInfo;
				componentLayouts.push_back(componentLayout);

			}


		}

		//sorting componentLayouts by alignment and then by size , ascending order

		std::sort(componentLayouts.begin(), componentLayouts.end(), [](const ComponentLayout& layoutA, const ComponentLayout& layoutB)
			{




				if (layoutA.alignment != layoutB.alignment)
				{
					if (layoutA.alignment > layoutB.alignment)
					{
						return true; // sort by alignment descending
					}
					else
					{
						return false;
					}
				}
				else
				{
					return layoutA.size > layoutB.size; // sort by size descending
				}

			});



		/****
		 Note :  For relative offsets to be correct and a common one for all same
		 archetype chunks of same size regardless of base address,
		 the base address has to be the common multiple of all alignment requirements of components,
		 we choose base address as multiple of 64 which can satisfy all typical alignment values.
		****/

		//check to see if alignment is power of two
		//since since for relative offets align properly we need power of two alignments
		for (const auto& layout : componentLayouts)
		{
			if (layout.alignment & (layout.alignment - 1))//check to see if alignment is power of two
			{
				throw std::runtime_error("Alignment must be a power of two");
			}
		}




		size_t sumOfAllComponentSizes = 0;
		for (auto& layout : componentLayouts)
		{
			sumOfAllComponentSizes += layout.size;
		}





		const size_t archetypeHeaderSize = sizeof(ArchetypeChunk);

		const size_t chunkCapacity = CHUNK_RAW_SIZE - archetypeHeaderSize;

		const size_t  maxNumOfEntities = chunkCapacity / sumOfAllComponentSizes; //with no padding

		size_t low = 0;
		size_t high = maxNumOfEntities;
		size_t mid = 0;
		size_t chunkEntityCapacity = 0;
		while (low <= high)
		{

			mid = low + (high - low) / 2; //mid is the number of entities in chunk

			if (checkFit(mid, CHUNK_RAW_SIZE, archetypeHeaderSize, componentLayouts))
			{
				chunkEntityCapacity = mid; //this is the maximum number of entities that can fit in chunk with this archetype definition
				low = mid + 1;



			}
			else
			{
				high = mid - 1;
			}

		}

		archetypeDefinition->componentLayouts = componentLayouts;

		archetypeDefinition->chunkRawSize = CHUNK_RAW_SIZE;
		archetypeDefinition->chunkEntityCapacity = chunkEntityCapacity;

		m_archetypeDefinitions.insert(signature, std::move(archetypeDefinition));


		auto iterator = m_archetypeDefinitions.find(signature);
		if (iterator != m_archetypeDefinitions.end())
		{
			return iterator->second.get();
		}


		return nullptr;

	}
















	void ArchetypeManager::updateChunkStatus(const ArchetypeSignature& signature)
	{

		//This function will categorize the chunks in the chunkList into available and full chunks based on their usage
		 //Potential edge cases to consider :



		//ToDo : get chunklist using archetype signature


		std::vector<ArchetypeChunk*> newAvailableChunkList;
		std::vector<ArchetypeChunk*> newFullChunkList;



		for (ArchetypeChunk* chunk : chunkList.availableChunks)
		{
			if (chunk != nullptr)
			{
				if (chunk->chunkEntityUsed >= chunk->chunkEntityCapacity)
				{
					newFullChunkList.push_back(chunk);
				}
				else
				{
					newAvailableChunkList.push_back(chunk);
				}
			}
		}

		for (ArchetypeChunk* chunk : chunkList.fullChunks)
		{
			if (chunk != nullptr)
			{
				if (chunk->chunkEntityUsed < chunk->chunkEntityCapacity)
				{
					newAvailableChunkList.push_back(chunk);
				}
				else
				{
					newFullChunkList.push_back(chunk);
				}
			}
		}

		chunkList.availableChunks = std::move(newAvailableChunkList);
		chunkList.fullChunks = std::move(newFullChunkList);



	}













	ArchetypeChunk* ArchetypeManager::getOrCreateFreeArchetypeChunk(const ArchetypeSignature& signature)
	{
		//Give out a available chunk if available, else create a new chunk of the given signature

		//Potential edge cases to consider
		// 1. If the signature is not found in m_archetypeDefinitions, then we need to create a new archetype definition first?


		auto it = m_chunkLists.find(signature);

		if (it != m_chunkLists.end()) // signature exsists in chunk lists
		{
			const ArchetypeDefinition* const archetypeDefinition = m_archetypeDefinitions[signature].get();
			ChunkList* const chunkList = &it->second; //get the chunk list for the given signature

			if (archetypeDefinition == nullptr)
			{
				//log archetype definition not found for the given signature
				//then exit? or return nullptr?

			}
			if (chunkList == nullptr)
			{
				//log chunk list not found for the given signature
				//then exit? or return nullptr?
			}

			//CategorizeChunks(chunkList);//update chunk lists to categorize available and full chunks


			//now check for avaiable chunks

			if (!chunkList.availableChunks.empty())
			{
				//if available chunks are present then return the last available chunk
				return chunkList.availableChunks.back(); //return last available chunk
			}
			else
			{
				//1.create new chunk 
				//2.place it in available chunks
				//3.return new chunk pointer

				ArchetypeChunk* newChunk = getNewChunk(signature);

				if (newChunk != nullptr)
				{
					chunkList.availableChunks.push_back(newChunk);
					return newChunk;
				}
				else
				{
					//log memory allocation failed for new chunk
					return nullptr; //or halt?

				}
			}


		}
		else
		{
			//signature not found in chunk lists, 
			//need to design a way to handle this case





		}






	}

	void ArchetypeManager::moveUpBetweenArchetypes(ArchetypeChunk* const srcArchetypeChunk, ArchetypeChunk* const destArchetypeChunk, const ComponentTypeInfo& componentTypeInfo, void* const component, EntityRecord& entityRecord)
	{




		//ToDo :  srcArchetypeChunk and destArchetypeChunk should be quired inside here so change the method signature
		//update entity record to reflect the new chunk here it self
		
		//ToDo : this is unsafe add nullptr checks
		std::vector<ComponentLayout> srcComponentLayout = srcArchetypeChunk->archetypeDefinition->componentLayouts;
		std::vector<ComponentLayout> destComponentLayout = destArchetypeChunk->archetypeDefinition->componentLayouts;
	
		
		std::vector<ComponentLayout&> srcSortedComponentLayout(srcComponentLayout.size());//consider making this vector pointer or reference


		for (size_t srcIndex = 0; srcIndex < srcComponentLayout.size(); ++srcIndex)
		{
			for (size_t destIndex = 0; destIndex < destComponentLayout.size(); ++destIndex)
			{
				if (srcComponentLayout[srcIndex].componentLayouts.id == destComponentLayout[destIndex].componentLayouts.id)
				{
					destSortedAccordingToSrcComponentLayout[srcIndex] = destComponentLayout[destIndex];//copying // consider pointer copying
				}
			}
		}





		//loop and move or copy,
		const size_t srcChunkBaseAddress = reinterpret_cast<size_t>();
		const size_t destChunkBaseAddress = reinterpret_cast<size_t>();

		const size_t destIndex = destArchetypeChunk->chunkEntityUsed;
		const size_t srcIndex = entityRecord.chunkIndex;


		for (size_t index = 0; index < destSortedAccordingToSrcComponentLayout.size(); ++index)
		{
			
			//move or copy
			const ComponentLayout& destsortedComponentLayout = destSortedAccordingToSrcComponentLayout[index]
			const size_t destBaseComponentOffset = destComponentLayout.offsetInChunk;
			const size_t srcBaseComponentOffset = srcComponentLayout[index].offsetInChunk;//check if index under size of the vector

			const size_t sizeOfcomponent = destsortedComponentLayout.size;

			//pointers to source and destination component location
			void* srcPtr = reinterpret_cast<void*>(srcChunkBaseAddress+ srcBaseComponentOffset+(sizeOfcomponent * srcIndex));
			void* destPtr = reinterpret_cast<void*>(destChunkBaseAddress+destBaseComponentOffset+(sizeOfcomponent * destIndex));

			//call method from type info
			const ComponentTypeInfo* destComponentTypeInfo = destsortedComponentLayout.componentTypeInfo;
			
			//check if nullptr

			destComponentTypeInfo.move(destPtr, srcPtr);//or do copy


		}

		for (size_t index = 0; index < destComponentLayout.size(); ++index)
		{
			if (destComponentLayout[index].componentTypeInfo.typeId == componentTypeInfo.typeId)
			{
				const ComponentLayout& destComponentLayout = destComponentLayout[index];
				const size_t destBaseComponentOffset = destComponentLayout.offsetInChunk;
				const size_t sizeOfcomponent = destComponentLayout.size;
				const void* destPtr = destChunkBaseAddress + destBaseComponentOffset + (destIndex* sizeOfcomponent);

				destComponentLayout[index].componentTypeInfo.move(destPtr, component);
			}
		}





		//src will have hole
		for (size_t index = 0; index < SrcComponentLayout.size(); ++index)
		{

			//move or copy
			const ComponentLayout& componentLayout = SrcComponentLayout[index]
			const size_t baseComponentOffset = componentLayout.offsetInChunk;
	

			const size_t sizeOfcomponent = componentLayout.size;

			//ToDo : correct it here
			void* srcPtr = reinterpret_cast<void*>(srcChunkBaseAddress + baseComponentOffset + (sizeOfcomponent * srcIndex));
			void* destPtr = reinterpret_cast<void*>(srcChunkBaseAddress + baseComponentOffset + (sizeOfcomponent * destIndex));

			//call method from type info
			const ComponentTypeInfo* destComponentTypeInfo = destsortedComponentLayout.componentTypeInfo;

			//check if nullptr

			destComponentTypeInfo.move(destPtr, srcPtr);//or do copy


		}


		//also update records and archetypes




		/*increase and decrease index here*/
		








		//ToDo : 
		//srcChunk will have a hole and that needs to be filled
		//and it will emmit back an id to be updated

		/*Fill the hole*/
			//Take last entity in source and fill in the hole and update the records
			



		//this requires more thought what if i store a pointer to the record instead
		//give out entity id to be updated(i am leaning towards pointer more )


	}
}
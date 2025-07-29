#include <type_traits>
#include "archetype_manager.h"
#include <vector>


namespace TheEngine::ECS
{


	bool ArchetypeManager::checkFit(const size_t numOfEntities, const size_t chunkRawSize, const size_t archetypeHeaderSize, std::vector<ComponentLayout>& componentLayouts)
	{

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



	ArchetypeChunk* getNewChunk(const ArchetypeSignature& signature)
	{
		//if last chunk is full then we need to create new chunk
		void* newChunk = m_chunkPoolAllocator.allocate(CHUNK_RAW_SIZE, alignof(ArchetypeChunk));

		ArchetypeChunk* archetypeChunk = new(newChunk) ArchetypeChunk();

		//assuming m_archetypeDefinitions[signature] is there
		archetypeChunk->archetypeDefinition = m_archetypeDefinitions[signature].get();

		archetypeChunk->chunkRawSize = CHUNK_RAW_SIZE;
		archetypeChunk->chunkEntityCapacity = archetypeChunk->archetypeDefinition.chunkEntityCapacity;

		archetypeChunk->chunkEntityUsed = 0;

		return archetypeChunk;

	}





	ArchetypeDefinition* ArchetypeManager::GetOrCreateArchetypeDefinition(const ArchetypeSignature& signature)
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
		for (size_t i = 0; i < signature.size(); ++i)
		{

			if (signature[i] == 1)
			{

				ComponentTypeInfo* componentTypeInfo = m_componentRegistry.getComponentTypeinfo(i); //get component type info
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



	void ArchetypeManager::CategorizeChunks(ChunkList& chunkList ) // temp method name , rename it to something better
	{

	

		std::vector<ArchetypeChunk*> newAvailableChunkList;
		std::vector<ArchetypeChunk*> newFullChunkList;



		for (ArchetypeChunk* chunk : chunkList.availableChunks)
		{
			if (chunk  != nullptr)
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













	ArchetypeChunk* ArchetypeManager::GetOrCreateChunk(const ArchetypeSignature& signature)
	{
		auto it = m_chunkLists.find(signature);







	E
		if (it != m_chunkLists.end())
		{
			if (!it->second.availableChunks.empty())
			{

				if (it->second.availableChunks.back().chunkEntityUsed < it->second.availableChunks.back().chunkEntityCapacity)
				{
					//take chunk from available chunks
					ArchetypeChunk* chunk = it->second.availableChunks.back();

					return chunk;
				}
				else
				{
					//check available list for any free chunks
					for (size_t i = it->second.availableChunks.size() - 1; i >= 0; --i)
					{
						//move chunk from available to full chunks
						ArchetypeChunk* chunk = it->second.availableChunks[i];//no bounds checking
						it->second.availableChunks.pop_back();

						it->second.fullChunks.push_back(chunk);


					}






				}
			}



		}

		//if not found then create new chunk with new archetype definition


	}






}
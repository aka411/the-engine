#pragma once
#include "common_data_types.h"
#include "component_registry.h"
#include "entity_manager.h"
#include "archetype_manager.h"
#include <cassert>
#include <stdint.h>

namespace TheEngine::ECS
{
	class ChunkView
	{
	private:
		size_t m_entityCount = 0;
		size_t m_archetypeChunkSize = 0;
		//ArchetypeChunkHeader* m_archetypeChunkHeader = nullptr;
		ArchetypeDefinition* m_archetypeDefinition = nullptr;
		//ArchetypeChunk* m_archetypeChunk = nullptr;
		std::uintptr_t m_chunkBaseAddress = 0;

	public:
		
		size_t getCount()
		{
			return m_entityCount;
		};

		ChunkView(ArchetypeChunkHeader* archetypeChunkHeader)
		{
			assert(archetypeChunkHeader != nullptr);
			assert(archetypeChunkHeader->archetypeChunk != nullptr);
			assert(archetypeChunkHeader->archetypeDefinition != nullptr);
			assert(archetypeChunkHeader->archetypeRecordChunk != nullptr);

			//m_archetypeChunkHeader = archetypeChunkHeader;
			m_entityCount = archetypeChunkHeader->chunkEntityUsed;
			m_archetypeDefinition = archetypeChunkHeader->archetypeDefinition;
			//m_archetypeChunk = archetypeChunkHeader->archetypeChunk;
			m_archetypeChunkSize = archetypeChunkHeader->archetypeDefinition->chunkRawSize;
			m_chunkBaseAddress = reinterpret_cast<std::uintptr_t>(archetypeChunkHeader->archetypeChunk);


		};

		template<typename ComponentType>
		ComponentType* getComponentArray(ComponentRegistry& componentRegistry);

	};



	template<typename ComponentType>
	ComponentType* ChunkView::getComponentArray(ComponentRegistry& componentRegistry)
	{
		ComponentId componentId = componentRegistry.getComponentIdFromComponent<ComponentType>();

		std::uintptr_t componentAddress = m_chunkBaseAddress + m_archetypeDefinition->GetComponentOffset(componentId);
		assert(componentAddress < m_archetypeChunkSize);
		return reinterpret_cast<ComponentType*>(componentAddress);
	}







	class EntityChunkView
	{
	private:
		
		size_t m_offsetIndex = 0;

		ArchetypeDefinition* m_archetypeDefinition = nullptr;
		std::uintptr_t m_chunkBaseAddress = 0;

	public:


		EntityChunkView(const EntityRecord& entityRecord)
		{
			assert(entityRecord.archetypeChunkHeader != nullptr);
			assert(entityRecord.archetypeChunkHeader->archetypeDefinition != nullptr);
	
	
			m_archetypeDefinition = entityRecord.archetypeChunkHeader->archetypeDefinition;
			m_offsetIndex = entityRecord.indexInArchetypeChunkRecordList;
			
			m_chunkBaseAddress = reinterpret_cast<std::uintptr_t>(entityRecord.archetypeChunkHeader->archetypeChunk);


		};

		template<typename ComponentType>
		ComponentType* getComponent(ComponentRegistry& componentRegistry);

	};



	template<typename ComponentType>
	ComponentType* EntityChunkView::getComponent(ComponentRegistry& componentRegistry)
	{
		ComponentId componentId = componentRegistry.getComponentIdFromComponent<ComponentType>();

		std::uintptr_t componentAddress = m_chunkBaseAddress + m_archetypeDefinition->GetComponentOffset(componentId)+sizeof(ComponentType) * m_offsetIndex;
		assert(componentAddress < m_archetypeDefinition->chunkRawSize+ m_chunkBaseAddress);

		return reinterpret_cast<ComponentType*>(componentAddress);
	}







    class Query
	{
	private:
		//using map would be better i think
		std::vector<ArchetypeChunkHeader*> m_archetypeChunkHeaders;

	public:

		void addArchetypeChunkHeaders(std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders);
	

		std::vector<ChunkView> getChunkView()
		{
			std::vector<ChunkView> chunkniew;
			return chunkniew;
		}


	};

	class QuerySystem
	{

	private:

		 ComponentRegistry& m_componentRegistry;
		 EntityManager& m_entityManager;
		 ArchetypeManager& m_archetypeManager;

	public:
		QuerySystem(ComponentRegistry& componentRegistry, EntityManager& entityManager, ArchetypeManager& archetypeManager);
		

		template<typename... ComponentType>
		Query getQuery();

		EntityChunkView getEntityChunkView(EntityId& entityId);

		ComponentRegistry& getComponentRegistry();
	
	};





	template<typename... ComponentType>
	inline Query QuerySystem::getQuery()
	{

		//process here 
		ArchetypeSignature neededArchtypeSignatureMask;

		(neededArchtypeSignatureMask.set(m_componentRegistry.getComponentIdFromComponent<ComponentType>()), ...);

	const std::unordered_map<ArchetypeSignature, ArchetypeDefinition*>& archetypeDefinitions = m_archetypeManager.getArchetypeDefinitions();

		//std::unordered_map<ArchetypeDefinition*, std::vector<ArchetypeChunkHeader*>> archetypeDefintionsToArchetypeChunkHeaders;
		Query query;
		

		for (const auto& it : archetypeDefinitions)
		{
			ArchetypeSignature archetypeSignature = it.first;

			if ((archetypeSignature & neededArchtypeSignatureMask) == neededArchtypeSignatureMask)
			{
				query.addArchetypeChunkHeaders(m_archetypeManager.getArchetypeChunksWithEntityData(archetypeSignature));
			}

		}
		
		return query;
	}

}
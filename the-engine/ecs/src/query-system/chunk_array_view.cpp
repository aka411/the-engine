#include "query-system/chunk_array_view.h"
#include <cassert>


namespace TheEngine::ECS
{
	ChunkArrayView::ChunkArrayView(ArchetypeChunkHeader* archetypeChunkHeader, const ComponentRegistry& componentRegistry) : m_componentRegistry(componentRegistry)
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

		//Chunk Record
		m_chunkRecordBaseAddress = reinterpret_cast<std::uintptr_t>(archetypeChunkHeader->archetypeRecordChunk);

	};


	const size_t ChunkArrayView::getCount() const
	{
		return m_entityCount;
	};


	const EntityId* ChunkArrayView::getChunkRecordArray() const
	{
		return reinterpret_cast<EntityId*> (m_chunkRecordBaseAddress);
	}

}
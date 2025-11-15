#include "query-system/entity_chunk_view.h"
#include <cassert>


namespace TheEngine::ECS
{


	EntityChunkView::EntityChunkView(const EntityRecord& entityRecord, const ComponentRegistry& componentRegistry) : m_componentRegistry(componentRegistry)
	{
		assert(entityRecord.archetypeChunkHeader != nullptr);
		assert(entityRecord.archetypeChunkHeader->archetypeDefinition != nullptr);


		m_archetypeDefinition = entityRecord.archetypeChunkHeader->archetypeDefinition;
		m_offsetIndex = entityRecord.indexInArchetypeChunkRecordList;

		m_chunkBaseAddress = reinterpret_cast<std::uintptr_t>(entityRecord.archetypeChunkHeader->archetypeChunk);


	};



}
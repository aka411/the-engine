#include "query-system/query.h"


namespace TheEngine::ECS
{
	Query::Query()
	{

	}




	void Query::addArchetypeChunkHeaders(std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders, const ComponentRegistry& componentRegistry)
	{
		m_chunkViews.reserve(archetypeChunkHeaders.size());

		for (const auto& archetypeChunkHeader : archetypeChunkHeaders)
		{

			m_chunkViews.emplace_back({ archetypeChunkHeader , componentRegistry });
		}
	}

	std::vector<ChunkView>& Query::getChunkArrayView()
	{
		
		return m_chunkViews;
	}


}
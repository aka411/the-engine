#include "query-system/query.h"


namespace TheEngine::ECS
{
	Query::Query()
	{

	}




	void Query::addArchetypeChunkHeaders(std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders, const ComponentRegistry& componentRegistry)
	{
		m_chunkArrayViews.reserve(archetypeChunkHeaders.size());

		for (const auto& archetypeChunkHeader : archetypeChunkHeaders)
		{

			m_chunkArrayViews.emplace_back( archetypeChunkHeader , componentRegistry );
		}
	}

	std::vector<ChunkArrayView>& Query::getChunkArrayViews()
	{
		
		return m_chunkArrayViews;
	}


}
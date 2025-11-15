#pragma once
#include <vector>
#include "chunk_array_view.h"

namespace TheEngine::ECS
{
	class Query
	{
	private:

		std::vector<ArchetypeChunkHeader*> m_archetypeChunkHeaders;
		std::vector<ChunkArrayView> m_chunkViews;

	public:

		Query();

		void addArchetypeChunkHeaders(std::vector<ArchetypeChunkHeader*> archetypeChunkHeaders,const ComponentRegistry& componentRegistry);

		std::vector<ChunkArrayView>& getChunkArrayView();



	};
}

#include "the-engine/terrain/terrain_generator.h"


void TerrainGenerator::setStrategy(std::unique_ptr<ITerrainGenerationStrategy> strategy)
{
	m_generationStrategy = std::move(strategy);
}

std::vector<float>  TerrainGenerator::generate(int width, int height, int seed)
{
	if (m_generationStrategy)
	{
		return m_generationStrategy->generateTerrain(width, height, seed);
	}
	// Handle case where no strategy is set (e.g., return empty or error)
	return {};
}

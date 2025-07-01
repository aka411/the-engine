#pragma once

#include <vector>
#include <memory>
#include "iterrain_generation_strategy.h"


class TerrainGenerator
{
private:
	std::unique_ptr<ITerrainGenerationStrategy> m_generationStrategy;

public:
	void setStrategy(std::unique_ptr<ITerrainGenerationStrategy> strategy);

	std::vector<float> generate(int width, int height, int seed);
};
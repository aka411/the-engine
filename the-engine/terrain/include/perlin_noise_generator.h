#pragma once

#include "iterrain_generation_strategy.h"

class PerlinNoiseGenerator : public ITerrainGenerationStrategy
{
public:
	std::vector<float> generateTerrain(int width, int height, int seed) override;
};


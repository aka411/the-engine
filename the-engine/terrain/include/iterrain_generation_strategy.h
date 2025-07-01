#pragma once

#include <vector>
//Statergy Pattern
class ITerrainGenerationStrategy
{
public:
	virtual ~ITerrainGenerationStrategy() = default;
	virtual std::vector<float> generateTerrain(int width, int height, int seed) = 0;
};
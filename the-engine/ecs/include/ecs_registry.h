#pragma once

class ECSRegistry
{

public:
	ECSRegistry() = default;
	~ECSRegistry() = default;
	// Add entity to the registry
	void addEntity(int entityID);
	// Remove entity from the registry
	void removeEntity(int entityID);
	// Check if an entity exists in the registry
	bool entityExists(int entityID) const;



};


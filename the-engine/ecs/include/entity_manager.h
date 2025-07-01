#pragma once

#include <vector>
#include <stack>
#include <the-engine/core/types.h>
class EntityManager
{
private:
	std::vector<bool> m_entities;//entity status list
	std::stack<int> m_availableEntityIds;//stack of available entities Id

public:


	TheEngine::EntityId createEntity();
	void destroyEntity(TheEngine::EntityId entityId);
	bool isAlive(TheEngine::EntityId entityId)const;




};


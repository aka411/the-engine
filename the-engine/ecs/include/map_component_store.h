#pragma once

#include "icomponent_store.h"

template <typename ComponentType>
class MapComponentStore : public IComponentStore<ComponentType>
{

public:
    // ... implementations for add, has, get, remove ...
    void removeComponentForEntity(const TheEngine::EntityId entityId) override;
	void addComponentForEntity(const TheEngine::EntityId entityId, const ComponentType& component) override;

private:
    std::unordered_map<TheEngine::EntityId, ComponentType> m_components;
};

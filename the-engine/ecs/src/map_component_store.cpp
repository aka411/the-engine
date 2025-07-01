#include "the-engine/ecs/map_component_store.h"


void MapComponentStore::removeComponentForEntity(TheEngine::EntityId entityId)
{
    m_components.erase(entityId);
}
void MapComponentStore::addComponentForEntity(const TheEngine::EntityId entityId, const ComponentType& component)
{
	m_components[entityId] = component;
}
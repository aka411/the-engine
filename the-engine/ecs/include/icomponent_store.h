#pragma once

#include "the-engine/core/types.h"


template<typename ComponentType>
class IComponentStore
{

public:

	

    virtual ~IComponentStore() = default;
    virtual void addComponentForEntity(const TheEngine::EntityId entityId, const ComponentType& component) = 0;
    virtual bool hasComponent(const TheEngine::EntityId entityId) const = 0;
    virtual ComponentType& getComponentForEntity(const TheEngine::EntityId entityId) = 0;
    virtual const ComponentType& getComponentForEntity(const TheEngine::EntityId entityId) const = 0;
    virtual void removeComponentForEntity(const TheEngine::EntityId entityId) = 0;

protected:


};


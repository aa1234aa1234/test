//
// Created by D412 on 2025-08-13.
//

#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H
#include <unordered_map>

#include "Types.h"

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
};

template<typename T>
class ComponentArray : public IComponentArray
{
    std::array<T, MAX_ENTITIES> components;
    std::unordered_map<EntityID,int> EntityToIndex;
    std::unordered_map<int,EntityID> IndexToEntity;
    int componentCount=0;
public:
    void Insert(EntityID entityID, T component)
    {
        EntityToIndex[entityID] = componentCount;
        IndexToEntity[componentCount] = entityID;
        components[componentCount] = component;
        componentCount++;
    }

    T& Get(EntityID entityID)
    {
        return components[EntityToIndex[entityID]];
    }

    bool Has(EntityID entityID) {
        return EntityToIndex.count(entityID);
    }

    void Remove(EntityID entityID)
    {
        components[EntityToIndex[entityID]] = components[componentCount-1];
        EntityToIndex[IndexToEntity[componentCount-1]] = EntityToIndex[entityID];
        IndexToEntity[EntityToIndex[entityID]] = IndexToEntity[componentCount-1];
        EntityToIndex.erase(entityID);
        IndexToEntity.erase(componentCount-1);
        componentCount--;
    }
};

#endif //COMPONENTARRAY_H

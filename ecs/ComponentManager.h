//
// Created by D412 on 2025-08-13.
//

#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include <iostream>
#include <ostream>

#include "ComponentArray.h"

class ComponentManager
{
    ComponentType componentType{};
    std::unordered_map<const char*, IComponentArray*> components{};
    std::unordered_map<const char*, ComponentType> componentTypes{};
    template<typename T>
    ComponentArray<T>* getComponentArray()
    {
        if (components.find(typeid(T).name()) == components.end()) return nullptr;
        return static_cast<ComponentArray<T>*>(components[typeid(T).name()]);
    }
public:
    ComponentManager() {}
    ~ComponentManager()
    {
        for (auto& p : components) delete p.second;
        components.clear();
    }

    template<typename T>
    T& getComponent(EntityID entity)
    {
        return getComponentArray<T>()->Get(entity);
    }

    template<typename T>
    void registerComponent()
    {
        components.insert({typeid(T).name(), new ComponentArray<T>()});
        componentTypes.insert({typeid(T).name(), componentType});
        ++componentType;
    }

    template<typename T>
    void removeComponent(EntityID entity)
    {
        getComponentArray<T>()->Remove(entity);
    }

    template<typename T>
    ComponentType getComponentType() {
        if (componentTypes.find(typeid(T).name()) == componentTypes.end()) { throw std::runtime_error("component type not found"); }
        return componentTypes[typeid(T).name()];
    }

    template<typename T>
    void addComponent(EntityID entity, T component)
    {
        getComponentArray<T>()->Insert(entity,component);
    }

    template<typename T>
    bool EntityHasComponent(EntityID entity) {
        return getComponentArray<T>()->Has(entity);
    }
};

#endif //COMPONENTMANAGER_H

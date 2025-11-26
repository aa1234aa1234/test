//
// Created by D412 on 2025-08-13.
//

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <unordered_map>

#include "Entity.h"
#include "System.h"

class System;

class SystemManager
{
    std::unordered_map<const char*, System*> systems;
    std::unordered_map<const char*, Signature> signatures;
public:
    ~SystemManager()
    {
        for (auto& p : systems)
        {
            delete p.second;
        }
        systems.clear();
    }

    template<typename T>
    T* registerSystem()
    {
        T* system = new T();
        if (systems.find(typeid(T).name()) != systems.end()) throw std::runtime_error("System already registered");
        systems[typeid(T).name()] = system;
        return system;
    }

    void updateEntitySignature(EntityID entity, Signature entitySignature)
    {
        for (auto& p : systems)
        {
            if ((entitySignature & signatures[p.first]) == signatures[p.first])
            {
                p.second->entities.insert(entity);
                std::cout << p.first << " " << p.second->entities.size() << " entities size" << std::endl;
            }
            else { p.second->entities.erase(entity); }
        }
    }

    template<typename T>
    void setSignature(Signature signature)
    {
        if (signatures.find(typeid(T).name()) != signatures.end()) throw std::runtime_error("Signature already set");
        signatures[typeid(T).name()] = signature;
    }

};

#endif //SYSTEMMANAGER_H

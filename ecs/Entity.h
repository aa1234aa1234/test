//
// Created by D412 on 2025-08-13.
//

#pragma once
#ifndef ENTITY_H
#define ENTITY_H

//#include "Engine.h"
#include "Types.h"

class Entity
{
    EntityID id=-1;
public:
    Entity()
    {
        static int id = 0;
        this->id = id++;
    }
    virtual ~Entity() = default;

    //virtual void render(Engine& engine);
    virtual void update(float deltatime);
    int getId() { return id; }
};

#endif //ENTITY_H

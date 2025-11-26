//
// Created by D412 on 2025-08-13.
//

#ifndef SYSTEM_H
#define SYSTEM_H
#include <set>

#include "Entity.h"

class System
{
public:
    std::set<EntityID> entities;
};

#endif //SYSTEM_H

//
// Created by sw_303 on 2025-08-30.
//

#ifndef UPDATESYSTEM_H
#define UPDATESYSTEM_H
#include "Components.hpp"
#include "SystemCoordinator.h"

class UpdateSystem : public System
{
public:
    UpdateSystem() {}
    ~UpdateSystem() {}

    void Initialize()
    {
        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<DirtyComponent>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<UpdateSystem>(signature);
    }

    void Update(float deltatime)
    {
        if (!entities.size()) return;
        std::vector<std::set<EntityID>::iterator> placeholder;
        auto it = entities.begin();
        for (; it != entities.end(); ++it)
        {
            SystemCoordinator::getInstance()->GetEntity(*it)->update(deltatime);
            //SystemCoordinator::getInstance()->RemoveComponent<DirtyComponent>(*it);
            placeholder.push_back(it);
            std::cout << "updatesystem" << std::endl;
        }
        for (auto& p : placeholder) SystemCoordinator::getInstance()->RemoveComponent<DirtyComponent>(*p);
    }
};

#endif //UPDATESYSTEM_H

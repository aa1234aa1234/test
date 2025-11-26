//
// Created by sw_303 on 2025-08-30.
//

#ifndef CLICKSYSTEM_H
#define CLICKSYSTEM_H
#include "Components.hpp"
#include "Input.h"
#include "SystemCoordinator.h"

class ClickSystem : public System
{
public:
    ClickSystem() {}
    ~ClickSystem() {}

    void Initialize()
    {
        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<ClickableComponent>(), true);
        signature.set(SystemCoordinator::getInstance()->GetComponentType<NonRenderableBoundingBox>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<ClickSystem>(signature);
    }

    void Update()
    {
        if (!Input::getInstance()->getEventType()[Input::EventType::MOUSE_DOWN]) return;

        for (auto& p : entities)
        {
            glm::vec2 mousepos = Input::getInstance()->getMousePos();
            auto box = SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(p);
            SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(p).onClick(p);
            if (mousepos.x >= box.boundingBox.x &&
                mousepos.x <= box.boundingBox.x+box.boundingBox.z &&
                mousepos.y >= box.boundingBox.y &&
                mousepos.y <= box.boundingBox.y+box.boundingBox.w) {
                //SystemCoordinator::getInstance()->GetComponent<ClickableComponent>(p).onClick(p);
            }
        }
    }
};

#endif //CLICKSYSTEM_H

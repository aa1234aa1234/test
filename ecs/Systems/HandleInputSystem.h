//
// Created by sw_306 on 2025-09-22.
//

#ifndef PROJECT_HANDLEINPUTSYSTEM_H
#define PROJECT_HANDLEINPUTSYSTEM_H
#include "Components.hpp"
#include "SystemCoordinator.h"

class HandleInputSystem : public System {
    EntityID selectedEntity=-1;
public:
    HandleInputSystem() {}
    ~HandleInputSystem() {}

    void Initialize() {
        auto* sc = SystemCoordinator::getInstance();
        Signature signature;
        signature.set(sc->GetComponentType<HandleInput>(), true);
        sc->SetSystemSignature<HandleInputSystem>(signature);
    }

    void Update(float deltatime) {
        //if (Input::getInstance()->getEvent() == -1) return;
        if (Input::getInstance()->isKeyDown(256) && selectedEntity > -1) {
            selectedEntity = -1;
            return;
        }
        if (Input::getInstance()->getEventType()[Input::EventType::MOUSE_DOWN]) {
            for (auto& p : entities) {
                auto transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(p);
                auto mousepos = Input::getInstance()->getMousePos();
                if (mousepos.x >= transform.position.x &&
                        mousepos.x <= transform.position.x+transform.size.x &&
                        mousepos.y >= transform.position.y &&
                        mousepos.y <= transform.position.y+transform.size.y) {
                    selectedEntity = p;
                    SystemCoordinator::getInstance()->GetEntity(p)->update(deltatime);
                    return;
                        }
            }
        }
        for (auto& p : entities) {
            if (selectedEntity == p) {
                SystemCoordinator::getInstance()->GetEntity(p)->update(deltatime);
                break;
            }
        }
    }
};

#endif //PROJECT_HANDLEINPUTSYSTEM_H
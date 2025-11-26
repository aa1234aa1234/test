//
// Created by sw_306 on 2025-08-27.
//

#ifndef PROJECT_SCROLLABLESYSTEM_H
#define PROJECT_SCROLLABLESYSTEM_H
#include "Components.hpp"
#include "Input.h"
#include "SystemCoordinator.h"

class ScrollableSystem : public System{
public:
    ScrollableSystem() {}
    ~ScrollableSystem() {}

    void Initialize() {
        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<ScrollableComponent>(),true);
        SystemCoordinator::getInstance()->SetSystemSignature<ScrollableSystem>(signature);
    }

    void Update() {
        if (Input::getInstance()->getEventType()[Input::EventType::SCROLL]) {
            bool shift = Input::getInstance()->isKeyDown(340);
            std::cout << shift << std::endl;
            for (auto& p : entities) {
                auto& scrollcomponent = SystemCoordinator::getInstance()->GetComponent<ScrollableComponent>(p);
                auto& transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(p);
                if (transform.position.x <= Input::getInstance()->getMousePos().x && Input::getInstance()->getMousePos().x <= transform.position.x+transform.size.x && transform.position.y <= Input::getInstance()->getMousePos().y && Input::getInstance()->getMousePos().y <= transform.position.y+transform.size.y) {
                    if (transform.size.y >= scrollcomponent.maxScroll*2) continue;
                    scrollcomponent.offset = scrollcomponent.offset+45*Input::getInstance()->getYOffset()*-1;
                    scrollcomponent.offset = scrollcomponent.offset < 0 ? 0 : scrollcomponent.offset;
                    scrollcomponent.offset = scrollcomponent.offset > scrollcomponent.maxScroll ? scrollcomponent.maxScroll : scrollcomponent.offset;
                    //scrollcomponent.offset += scrollcomponent.offset+Input::getInstance()->getYOffset()/10.0 < 0 || scrollcomponent.offset+Input::getInstance()->getYOffset()/10.0 > 1 ? 0 : Input::getInstance()->getYOffset()/10.0;
                }
            }
        }
    }
};

#endif //PROJECT_SCROLLABLESYSTEM_H
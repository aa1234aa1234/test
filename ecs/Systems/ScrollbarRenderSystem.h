//
// Created by admin on 2025-08-22.
//

#ifndef PROJECT_SCROLLBARRENDERSYSTEM_H
#define PROJECT_SCROLLBARRENDERSYSTEM_H
#include "Components.hpp"
#include "System.h"
#include "SystemCoordinator.h"

class ScrollbarRenderSystem : public System {
    int width,height;
public:
    ScrollbarRenderSystem() {}
    ~ScrollbarRenderSystem() {}

    void Initialize(int& width, int& height) {
        this->width = width, this->height = height;

        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<ScrollableComponent>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<ScrollbarRenderSystem>(signature);
    }

    void Update() {
        for (auto& p : entities) {
            if (!SystemCoordinator::getInstance()->EntityHasComponent<TransformComponent>(p)) continue;

        }
    }
};

#endif //PROJECT_SCROLLBARRENDERSYSTEM_H
//
// Created by admin on 2025-08-22.
//

#ifndef PROJECT_SCROLLBAR_H
#define PROJECT_SCROLLBAR_H
#include "Components.hpp"
#include "SystemCoordinator.h"
#include "glm/detail/type_vec.hpp"

class ScrollBar : public Entity {
    static const int ScrollBarWidth = 15;
public:
    ScrollBar() {}
    ~ScrollBar() {}

    void Initialize(glm::vec2 position, glm::vec2 size) {
        SystemCoordinator::getInstance()->RegisterEntity(this);
        SystemCoordinator::getInstance()->AddComponent(getId(), TransformComponent{glm::vec2(position.x+size.x-ScrollBarWidth, position.y), glm::vec4(60,60,60,0.5), glm::vec2(ScrollBarWidth,size.y)});
    }
};

#endif //PROJECT_SCROLLBAR_H
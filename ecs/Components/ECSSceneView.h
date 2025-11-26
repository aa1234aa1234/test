//
// Created by sw_306 on 2025-09-15.
//

#ifndef PROJECT_ECSSCENEVIEW_H
#define PROJECT_ECSSCENEVIEW_H
#include "Components.hpp"
#include "SystemCoordinator.h"
#include "Components/CameraComponent.h"


class ECSSceneView : public Entity{
    CameraComponent* camera;
public:
    ECSSceneView(unsigned int texture,glm::vec2 position, glm::vec2 size, glm::vec4 color, CameraComponent* camera) {
        this->camera = camera;
        Initialize(texture,position,size,color);
    }
    ~ECSSceneView() {

    }

    void Initialize(unsigned int texture,glm::vec2 position, glm::vec2 size, glm::vec4 color) {
        auto* sc = SystemCoordinator::getInstance();
        sc->RegisterEntity(this);
        sc->AddComponent(getId(), TextureComponent{texture});
        sc->AddComponent(getId(), TransformComponent{position, color, size});
        sc->AddComponent(getId(), HandleInput{});
    }

    void update(float deltatime) override {

        auto transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(getId());
        auto mousepos = Input::getInstance()->getMousePos();
        if (mousepos.x >= transform.position.x &&
                mousepos.x <= transform.position.x+transform.size.x &&
                mousepos.y >= transform.position.y &&
                mousepos.y <= transform.position.y+transform.size.y) {
            camera->update(deltatime);
        }

    }

};

#endif //PROJECT_ECSSCENEVIEW_H
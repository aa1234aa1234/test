//
// Created by sw_306 on 2025-09-17.
//

#ifndef PROJECT_SCENEVIEWRENDERER_H
#define PROJECT_SCENEVIEWRENDERER_H
#include "Components.hpp"
#include "SystemCoordinator.h"

class SceneViewRenderer : public System {
    unsigned int vao,vbo;
    int width, height;
    Shader* shader;

    struct Element {
        glm::vec2 position, size;
    };

public:
    SceneViewRenderer() {}
    ~SceneViewRenderer() {
        delete shader;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void Initialize(int& width, int& height) {
        this->width = width, this->height = height;
        shader = new Shader("resources/shader/editor/scenevert.glsl", "resources/shader/editor/scenefrag.glsl");

        float ndc[] = {
            1.0, 1.0 ,
            1.0,-1.0,
            -1.0,1.0,
            -1.0,-1.0,
            1.0,-1.0,
            -1.0,1.0,
        };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ndc), ndc, GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<TextureComponent>(), true);
        signature.set(SystemCoordinator::getInstance()->GetComponentType<TransformComponent>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<SceneViewRenderer>(signature);


    }

    void Update() {
        shader->use();
        for (auto& p : entities) {
            glBindTexture(GL_TEXTURE_2D, SystemCoordinator::getInstance()->GetComponent<TextureComponent>(p).texture);
            auto& transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(p);
            float aspect = (float)width / (float)height;
            float ratio = transform.size.x / transform.size.y;
            glm::vec2 scale{};
            if (aspect > ratio) { scale.y = aspect - ratio; }
            glm::mat4 mat = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
            mat = glm::translate(mat, glm::vec3(glm::vec2(transform.position.x, transform.position.y + (height-transform.size.y * (1/aspect))/2), 0.0f));
            mat = glm::scale(mat, glm::vec3(transform.size, 1.0f));
            mat = glm::scale(mat, glm::vec3(1, 1/aspect, 1));
            glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(mat));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
};

#endif //PROJECT_SCENEVIEWRENDERER_H
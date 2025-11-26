//
// Created by sw_306 on 2025-08-20.
//

#ifndef PROJECT_RENDERSYSTEM_H
#define PROJECT_RENDERSYSTEM_H
#include "Components.hpp"
#include "System.h"
#include "SystemCoordinator.h"

class RenderSystem : public System {
    unsigned int vao,vbo,instancevbo;
    Shader* shader = nullptr;
    int width,height;
public:

    RenderSystem() {

    }
    ~RenderSystem() {
        delete shader;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &instancevbo);
    }

    void Initialize(int& width, int& height) {
        this->width = width;
        this->height = height;
        shader = new Shader("resources/shader/editor/editorvert.glsl","resources/shader/editor/editorfrag.glsl");

        shader->use();
        glUniform1f(glGetUniformLocation(shader->getId(), "width"), width);
        glUniform1f(glGetUniformLocation(shader->getId(), "height"), height);

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
        glGenBuffers(1, &instancevbo);
        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glBufferData(GL_ARRAY_BUFFER, MAX_COMPONENTS * sizeof(TransformComponent), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ndc), ndc, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TransformComponent), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(TransformComponent), (void*)(offsetof(TransformComponent, color)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TransformComponent), (void*)(offsetof(TransformComponent, size)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(TransformComponent), (void*)(offsetof(TransformComponent, clipRect)));
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);





        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);


        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<TransformComponent>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<RenderSystem>(signature);
    }

    void Update() {
        std::vector<TransformComponent> data;
        std::set<EntityID>::iterator it;
        for (it = entities.begin(); it != entities.end(); it++) {
            if (it == entities.end()) break;
            auto transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(*it);
            data.push_back(transform);
            //std::cout << "transform component: " << transform.position.x << std::endl;
        }

        shader->use();
        glm::mat4 mat = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(mat));

        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(TransformComponent), data.data());

        glBindVertexArray(vao);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, data.size());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

#endif //PROJECT_RENDERSYSTEM_H
//
// Created by sw_306 on 2025-08-25.
//

#ifndef PROJECT_TREENODERENDERSYSTEM_H
#define PROJECT_TREENODERENDERSYSTEM_H
#include "Components.hpp"
#include "ECSAssetBrowser.h"
#include "SystemCoordinator.h"
#include "TextHandler.h"

class TreeNodeRenderSystem : public System{
    unsigned int vao,vbo,instancevbo;
    Shader* shader = nullptr;
    int width,height;

    struct Element {
        glm::vec2 position;
        glm::vec2 size;
        bool selected=false;
    };

    std::vector<Element> instanceData;

public:
    TreeNodeRenderSystem() {}
    ~TreeNodeRenderSystem() {
        delete shader;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &instancevbo);
    }

    void Initialize(int& width, int& height) {
        this->width = width;
        this->height = height;
        shader = new Shader("resources/shader/editor/treenodevert.glsl","resources/shader/editor/treenodefrag.glsl");

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
        glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Element), NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ndc), ndc, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Element), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribDivisor(1, 1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Element), (void*)(offsetof(Element, size)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);

        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Element), (void*)(offsetof(Element, selected)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);


        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        Signature signature;
        signature.set(SystemCoordinator::getInstance()->GetComponentType<TreeNodeComponent>(), true);
        SystemCoordinator::getInstance()->SetSystemSignature<TreeNodeRenderSystem>(signature);

        instanceData.reserve(1000);
    }

    void Update(const EntityID treeView) {
        shader->use();
        glm::mat4 mat = glm::ortho(0.0f, static_cast<float>(this->width), static_cast<float>(this->height), 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(mat));
        int elementCount = 0;
        instanceData.clear();
        std::vector<Text> texts;
        auto transform = SystemCoordinator::getInstance()->GetComponent<TransformComponent>(treeView);
        auto scrollcomponent = SystemCoordinator::getInstance()->GetComponent<ScrollableComponent>(treeView);
        auto entities = SystemCoordinator::getInstance()->GetComponent<ContentComponent>(treeView);
        glScissor(transform.position.x, height-(transform.position.y+transform.size.y), transform.size.x, transform.size.y);
        float offset = scrollcomponent.offset;
        for (auto& p : entities.entities) {
            auto treenode = SystemCoordinator::getInstance()->GetComponent<TreeNodeComponent>(p);
            if (!treenode.visible) continue;
            auto position = SystemCoordinator::getInstance()->GetComponent<PositionComponent>(p);
            if (position.position.y-offset < -(ROWHEIGHT) || position.position.y-offset > transform.position.y+transform.size.y) continue;
            std::string text = SystemCoordinator::getInstance()->GetComponent<TextComponent>(p).text;
            instanceData.emplace_back(Element{glm::vec2(transform.position.x+STARTING_OFFSETX,position.position.y-offset),glm::vec2(transform.size.x, ROWHEIGHT),treenode.selected});
            texts.push_back(Text{position.position.x+TABWIDTH+1, position.position.y+2-offset, text});
            elementCount++;
        }

        glBindBuffer(GL_ARRAY_BUFFER, instancevbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, elementCount * sizeof(Element), instanceData.data());

        glBindVertexArray(vao);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, elementCount);

        for (auto& p : texts) {
            TextHandler::getInstance()->manualDrawText(p);
        }


    }
};

#endif //PROJECT_TREENODERENDERSYSTEM_H

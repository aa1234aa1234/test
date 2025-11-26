//
// Created by D412 on 2025-08-13.
//
#pragma once
#ifndef COMPONENTS_H
#include <functional>
#include <vector>

#include "Entity.h"

struct TransformComponent
{
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 size;
    glm::vec4 clipRect = glm::vec4(-1, -1, -1, -1);
};

struct ContentComponent
{
    std::vector<EntityID> entities;
};

struct ScrollableComponent
{
    bool horizontal, vertical;
    float offset;
    int maxScroll;
    int scrollBarId = -1;
};


struct TreeNodeComponent {
    bool expanded = false, selected = false, visible = false;
    int textIndex = -1;
    EntityID treeView = -1;
};

struct PositionComponent {
    glm::vec2 position;
};

struct TextComponent {
    std::string text;
};

struct HoverableComponent {
    unsigned int hoverShader;
};

struct ClickableComponent
{
    glm::vec4 boundingBox;
    std::function<void(EntityID)> onClick;
};

//maybe combine with TextureComponent on later date
struct RenderableIcon
{
    glm::vec4 uvRect;
    glm::vec4 boundingBox;
    glm::vec4 renderRect;
    bool visible = false;
};

struct DirtyComponent {};

struct ParentComponent {
    EntityID parent=-1;
};

struct TextureComponent {
    unsigned int texture;
};

struct NonRenderableBoundingBox {
    glm::vec4 boundingBox;
};

struct HandleInput {
};

#define COMPONENTS_H

#endif //COMPONENTS_H

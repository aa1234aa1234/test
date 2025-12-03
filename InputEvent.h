//
// Created by D412 on 2025-07-07.
//
#pragma once
#ifndef INPUTEVENT_H
#include <glm/glm.hpp>
class InputEvent
{
    int key;
    double xoffset,yoffset;
    glm::vec2 mousePos, mouseDelta;
    int eventType=-1;
	int mouseType = -1;
    bool active=false;
public:
    InputEvent() {}
    InputEvent(int key, int eventType) : key(key), eventType(eventType) {}
    InputEvent(glm::vec2 mousePos, int eventType, int mouseType = 0) : mousePos(mousePos), eventType(eventType), mouseType(mouseType) {}
    InputEvent(glm::vec2 mousePos, glm::vec2 mouseDelta, int eventType, int mouseType = 0) : mousePos(mousePos), mouseDelta(mouseDelta), eventType(eventType), mouseType(mouseType) {}
    InputEvent(double x, double y, int eventType) : xoffset(x), yoffset(y), eventType(eventType) {}
    ~InputEvent() {}

    int getEventType() { return eventType; }
    glm::vec2 getMousePos() { return mousePos; }
    glm::vec2 getMouseDelta() { return mouseDelta; }
    glm::vec2 getScrollDelta() { return glm::vec2(xoffset,yoffset); }
	int getMouseType() { return mouseType; }
    int getKey() { return key; }
    bool isActive() { return active; }
    void setActive(bool active) { this->active = active; }
};
#define INPUTEVENT_H

#endif //INPUTEVENT_H

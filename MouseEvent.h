//
// Created by sw_303 on 2025-06-28.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include "EventHandler.h"
#include <glm/glm.hpp>

class MouseEvent : public IEvent {
    int id = -1;
    glm::vec2 mousePos, mouseOffset;
	int mouseType=0;
public:
    enum MouseEventType
    {
        MOUSEDOWN=0,
        MOUSEUP,
        MOUSEMOVE,
        MOUSEDRAG
    } eventType;

    MouseEvent() {}
    MouseEvent(glm::vec2 mousePos, glm::vec2 mouseOffset, MouseEventType eventType, int mouseType = 0) : mousePos(mousePos), mouseOffset(mouseOffset), eventType(eventType), id(eventType), mouseType(mouseType) {}
    MouseEvent(glm::vec2 mousePos, MouseEventType eventType, int mouseType = 0) : mousePos(mousePos), eventType(eventType), id(eventType), mouseType(mouseType) {}
    ~MouseEvent() {}
    const int getId() override
    {
        return id;
    }

    std::type_index getType() override
    {
        return typeid(MouseEvent);
    }

    glm::vec2 getMousePos() { return mousePos; }
    glm::vec2 getMouseOffset() { return mouseOffset; }
    MouseEventType getEventType() { return eventType; }
	int getMouseType() { return mouseType; }
};



#endif //MOUSEEVENT_H

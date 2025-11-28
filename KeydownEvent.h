//
// Created by D412 on 2025-06-30.
//

#ifndef KEYDOWNEVENT_H
#define KEYDOWNEVENT_H
#include "EventHandler.h"
#include "EventDispatcher.h"

class KeydownEvent : public IEvent{


    int id=-1;
    int pressedkey = 0;
public:
    enum KeyDown
    {
        KEY_DOWN = 4,
        KEY_UP
    } eventType;
    KeydownEvent(int keycode, int id) : id(id) {}
    const int getId() override
    {
        return id;
    }
    std::type_index getType() override
    {
        return typeid(KeydownEvent);
    }
    int getKeycode() { return pressedkey; }
    int getEventType() { return eventType; }
};



#endif //KEYDOWNEVENT_H

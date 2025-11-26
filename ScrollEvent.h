//
// Created by D412 on 2025-08-13.
//

#ifndef SCROLLEVENT_H
#define SCROLLEVENT_H

class ScrollEvent
{
    double xoffset, yoffset;

    int eventType = -1;
public:
    enum ScrollEventType
    {
        SCROLL = 6
    };
    ScrollEvent(double xoffset, double yoffset) : xoffset(xoffset), yoffset(yoffset) {}

    double getXOffset() { return xoffset; }
    double getYOffset() { return yoffset; }
};

#endif //SCROLLEVENT_H

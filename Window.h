//
// Created by user on 6/15/2025.
//

#ifndef WINDOW_H
#define WINDOW_H
#include "header.h"


class Window {
    GLFWwindow* window;
    int screen_width, screen_height;
public:
    Window();
    Window(const int& width, const int& height, std::string title);
    ~Window();

    void swapBuffers();
    void init(const int& width, const int& height, std::string title);
    int getWidth() const;
    int getHeight() const;

    GLFWwindow* getWindow() const;
};



#endif //WINDOW_H

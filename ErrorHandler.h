//
// Created by Administrator on 2025-10-18.
//
#pragma once
#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H
#include <iostream>
//#include <GLFW/glfw3.h>

#define GLCall(x)   \
    {              \
        glClearError(); \
        x;              \
        glLogError();   \
    }                 \

inline void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

inline void glLogError() {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "[OpenGL Error] " << error << '\n';
    }
}

#endif //ERRORHANDLER_H

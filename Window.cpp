//
// Created by user on 6/15/2025.
//

#include "Window.h"

#include "Engine.h"
#include "stb/stb_image.h"

Window::Window() {}

Window::Window(const int& width, const int& height, std::string title) {
    init(width, height, title);
}

Window::~Window() {
    if (window != nullptr) {
        glfwTerminate();
    }
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
    glFinish();
}

int Window::getWidth() const { return screen_width; }
int Window::getHeight() const { return screen_height; }

void Window::init(const int& width, const int& height, std::string title)
{
    screen_width = width; screen_height = height;
    if (!glfwInit())
        return;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_set_flip_vertically_on_load(true);
}

GLFWwindow* Window::getWindow() const { return window; }

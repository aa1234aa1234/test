//
// Created by D412 on 2025-06-16.
//

#include "Application.h"
#include "Game.h"


#include "KeydownEvent.h"
#include "MouseEvent.h"
#include "ScrollEvent.h"
#include "Input.h"


void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputEvent keyEvent = InputEvent(key, action == GLFW_RELEASE ? KeydownEvent::KeyDown::KEY_UP : KeydownEvent::KeyDown::KEY_DOWN);
    keyEvent.setActive(true);
    inputEvent[keyEvent.getEventType()].push(keyEvent);
}

void Application::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if (firstMouse) { lastMousePos = glm::vec2(xpos, ypos); firstMouse = false; }
    InputEvent mouseEvent =InputEvent(glm::vec2(xpos,ypos), action == GLFW_RELEASE ? MouseEvent::MouseEventType::MOUSEUP : MouseEvent::MouseEventType::MOUSEDOWN, button);
    mouseEvent.setActive(true);
    inputEvent[mouseEvent.getEventType()].push(mouseEvent);
    if (action == GLFW_RELEASE) firstMouse = true;
}

void Application::cursorpos_callback(GLFWwindow* window, double xpos, double ypos)
{
    int action=0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != 1) action = 1;
    InputEvent mouseEvent = InputEvent(glm::vec2(xpos,ypos), lastMousePos-glm::vec2(xpos,ypos), action ? MouseEvent::MouseEventType::MOUSEMOVE : MouseEvent::MouseEventType::MOUSEDRAG);
    inputCnt[mouseEvent.getEventType()]++;
    mouseEvent.setActive(true);
    inputEvent[mouseEvent.getEventType()].push(mouseEvent);
    lastMousePos = glm::vec2(xpos,ypos);
}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    InputEvent scrollEvent = InputEvent(xoffset,yoffset, ScrollEvent::ScrollEventType::SCROLL);
    scrollEvent.setActive(true);
    inputEvent[scrollEvent.getEventType()].push(scrollEvent);
}

void Application::error_callback(void* user_data, int error, const char* description) {
    std::cerr << error << ' ' << description << '\n';
}

Application::Application()
{
    //root.addSceneNode(node);
}

Application::~Application()
{
	delete game;
}

void Application::initialize() {
	int width = 1200;
	int height = 900;
	game = new Game(width,height);
}

void Application::setCallBack(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window,keyCallback);
    glfwSetMouseButtonCallback(window,mouseCallback);
    glfwSetCursorPosCallback(window,cursorCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetErrorCallback(globalErrorHandler);
    ErrorHandler errorHandler = error_handler;
    errorHandler.user_data = this;
    errorHandler.handler = [](void* user_data, int error, const char* description) {
        std::cout << error << ' ' << description << '\n';
    };
    glfwRequestWindowAttention(window);
    error_handler = errorHandler;
}

void Application::render()
{
    //glClearColor(0.0, 1.0, 1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	game->render();
}

void Application::handleInput(float deltatime)
{
    
}

void Application::update(float deltatime)
{
    game->update();
}

void Application::popEvent() {
    for (int i = 0; i<inputEvent.size(); i++) {
        if (inputEvent[i].size()) inputEvent[i].pop();
    }
}




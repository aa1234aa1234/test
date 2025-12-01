//
// Created by user on 6/15/2025.
//

#include "Engine.h"
#include "Application.h"
#include "FrameBuffer.h"

#include "Input.h"
#include "KeydownEvent.h"
#include "MouseEvent.h"

int Engine::screenHeight, Engine::screenWidth;
bool Engine::isRunning = false;
bool Engine::editorMode = false;

Engine::Engine(Application* app, const int& width, const int& height, const std::string& title) : window(), application(app) {
    screenWidth = width;
    screenHeight = height;
    window.init(width,height,title);
    glfwSwapInterval(1);

    eventDispatcher = new EventDispatcher();
    application->setCallBack(window.getWindow());
	application->initialize();
    sceneBuffer = new FrameBuffer(screenWidth,screenHeight);


}

Engine::~Engine()
{
    delete application;
    delete sceneBuffer;
}

void Engine::run()
{
    float lastframe = 0.0;
    //int fps = TextHandler::getInstance()->addText(10, 10, "");
    float frames = 0;
    char buf[1024];
    while (isRunning)
    {
        //frameLimiter.begin();
        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastframe;
        lastframe = currentFrame;

        sprintf(buf, "FPS: %.1f", frames);
        //TextHandler::getInstance()->editText(1000, 860, buf, fps, 1.0);

        glfwPollEvents();

        glDisable(GL_DEPTH_TEST);
        glClearColor(0.09, 0.09, 0.09, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //normal engine code holy shit im making comments omg
        InputEvents event;
        application->pollInputEvent(event);

        //------------           please refactor this holy shit           ---------------------
        // handleInput(deltatime,event.keyDown);
        // handleInput(deltatime,event.keyUp);
        // handleInput(deltatime,event.scroll);
        handleInput(deltatime,event.mouseDown);
        // handleInput(deltatime,event.mouseUp);
        // handleInput(deltatime,event.mouseDrag);
        // handleInput(deltatime,event.mouseMove);
        application->handleInput(deltatime);
        application->update(deltatime);
        render(deltatime);

        Input::getInstance()->setEvent(-1);
        for (auto& p : Input::getInstance()->getEventType())
        {
            Input::getInstance()->setEventType(p.first,false);
        }
        if (Input::getInstance()->isKeyDown('A'))
        {
            std::cout << "123" << std::endl;
        }
        //frames = frameLimiter.end();
        //Input::getInstance()->setEventType(-1);
        //ill leave this here need to making editor layer
        window.swapBuffers();
    }
    glfwTerminate();
}

void Engine::render(float deltatime)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    application->render();

}

void Engine::handleInput(float deltatime, InputEvent event)
{
    if (event.getEventType() != -1) {
		Input::getInstance()->setEvent(event.getEventType());
	}
    switch (event.getEventType())
    {
    case Input::EventType::MOUSE_DOWN:
        {
            Input::getInstance()->setEventType(Input::EventType::MOUSE_DOWN, event.isActive());
            Input::getInstance()->setMousePos(event.getMousePos());
            Input::getInstance()->setKeyDown(LEFT_MOUSE_BUTTON + event.getMouseType(), event.isActive());
			Input::getInstance()->setMouseType(event.getMouseType());
            if (event.isActive())
            {
                MouseEvent mouseEvent = MouseEvent(event.getMousePos(), event.getMouseDelta(), MouseEvent::MouseEventType::MOUSEDOWN, event.getMouseType());
                eventDispatcher->dispatchEvent(mouseEvent);
            }

        }
        break;
    case Input::EventType::MOUSE_UP:
        {
            if (event.isActive())
            {
                MouseEvent mouseEvent = MouseEvent(event.getMousePos(), MouseEvent::MouseEventType::MOUSEUP, event.getMouseType());
                eventDispatcher->dispatchEvent(mouseEvent);
            }

            Input::getInstance()->setEventType(Input::EventType::MOUSE_UP, event.isActive());
            Input::getInstance()->setMousePos(event.getMousePos());
			Input::getInstance()->setMouseType(event.getMouseType());
            Input::getInstance()->setKeyDown(LEFT_MOUSE_BUTTON + event.getMouseType(), !event.isActive());
        }
        break;
    case Input::EventType::MOUSE_MOVE:
        {
            if (event.isActive())
            {
                MouseEvent mouseEvent = MouseEvent(event.getMousePos(), event.getMouseDelta(), MouseEvent::MouseEventType::MOUSEMOVE);
                eventDispatcher->dispatchEvent(mouseEvent);
            }

            Input::getInstance()->setEventType(Input::EventType::MOUSE_MOVE, event.isActive());
            Input::getInstance()->setMousePos(event.getMousePos());
            Input::getInstance()->setMouseDelta(event.getMouseDelta());
        }

        break;
    case Input::EventType::MOUSE_DRAG:
        {
            if (event.isActive())
            {
                MouseEvent mouseEvent = MouseEvent(event.getMousePos(), event.getMouseDelta(), MouseEvent::MouseEventType::MOUSEDRAG, event.getMouseType());
                eventDispatcher->dispatchEvent(mouseEvent);
            }

            Input::getInstance()->setEventType(Input::EventType::MOUSE_DRAG, event.isActive());
            Input::getInstance()->setMousePos(event.getMousePos());
            Input::getInstance()->setMouseDelta(event.getMouseDelta());
        }
        break;
    case Input::EventType::KEY_DOWN:
        {
            Input::getInstance()->setEventType(Input::EventType::KEY_DOWN, event.isActive());
            Input::getInstance()->setKeyDown(event.getKey(), event.isActive());
            if (event.isActive())
            {
                KeydownEvent keydown = KeydownEvent(event.getKey(),Input::EventType::KEY_DOWN);
                eventDispatcher->dispatchEvent(keydown);
            }

        }
        break;
    case Input::EventType::KEY_UP:
        {
            Input::getInstance()->setEventType(Input::EventType::KEY_UP, event.isActive());
            Input::getInstance()->setKeyDown(event.getKey(), !event.isActive());
            if (event.isActive())
            {
                KeydownEvent keydown = KeydownEvent(event.getKey(),Input::EventType::KEY_UP);
                eventDispatcher->dispatchEvent(keydown);
            }

        }
        break;
    case Input::EventType::SCROLL:
        {
            Input::getInstance()->setXOffset(event.getScrollDelta().x);
            Input::getInstance()->setYOffset(event.getScrollDelta().y);
            Input::getInstance()->setEventType(Input::EventType::SCROLL,event.isActive());
        }
        break;
    }
    if (application->getInputEvents().size()) application->popEvent();
}

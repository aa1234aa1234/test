//
// Created by user on 6/15/2025.
//

#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#define FRAME_LIMIT 144

class Application;
class FrameBuffer;
class EventDispatcher;
class Window;

class Engine {


    static int screenWidth, screenHeight;
    static bool isRunning, editorMode;
    float deltatime=0.0;
    Window window;
    Application* application=nullptr;
    FrameBuffer* sceneBuffer=nullptr;
    EventDispatcher* eventDispatcher=nullptr;

public:
    Engine();
    Engine(Application* app, const int& width, const int& height, const std::string& title);
    ~Engine();

    static int getScreenWidth() { return screenWidth; }
    static int getScreenHeight() { return screenHeight; }
    bool running() { return isRunning; }
    static void setMode(const bool& mode) { editorMode = mode; }
    static void setRunning(const bool& running) { isRunning = running; }
    static bool getMode() { return editorMode; }
    float getDeltaTime() { return deltatime; }
    FrameBuffer* getSceneBuffer() { return sceneBuffer; }
    EventDispatcher* getEventDispatcher() { return eventDispatcher; }
    Application* getApplication() { return application; }
    Window& getWindow() { return window; }

    void run();
    void render(float deltatime);
    void handleInput(float deltatime, InputEvent event);
};

#endif //ENGINE_H

//
// Created by D412 on 2025-06-30.
//

#include "testing.h"

#include "Engine.h"
#include "TextHandler.h"

int main(int argc, char** argv)
{
    Application application;
    Engine engine(&application,1200,900,"Engine");
    Engine::setMode(true);
    Engine::setRunning(true);
    engine.run();
}

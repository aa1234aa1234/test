//
// Created by D412 on 2025-06-30.
//

#include "testing.h"

#include "Engine.h"
#include "Application.h"

int main(int argc, char** argv)
{
	std::cout << argv[0] << std::endl;
    Application application;
    Engine engine(&application,1200,900,"Game");
    Engine::setMode(true);
    Engine::setRunning(true);
    engine.run();
}

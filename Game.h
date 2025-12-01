#pragma once
#include "SystemCoordinator.h"
#include "IconRenderSystem.h"
#include "ClickSystem.h"
#define CELL_SIZE 20

class Game {
	EntityID cells[30][16];
	IconRenderSystem* iconrenderer;
	ClickSystem* clicksystem;
public:
	Game(int& width, int& height) {
		iconrenderer = new IconRenderSystem();
		clicksystem = new ClickSystem();
		iconrenderer->Initialize(width, height);
		clicksystem->Initialize();
		Initialize();
	}
	~Game() {
		delete iconrenderer;
		delete clicksystem;
	}

	void Initialize();
	void update();
	void render();
	void checkCells(int x, int y);
};

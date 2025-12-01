#pragma once
#include "SystemCoordinator.h"
#include "IconRenderSystem.h"
#include "ClickSystem.h"
#define CELL_SIZE 35


struct CellComponent {
	int tileid = 0;
	bool isRevealed = false;
};

class Game {
	EntityID cells[30][16];
	IconRenderSystem* iconrenderer;
	ClickSystem* clicksystem;
	glm::vec2 lastclick;

	enum TileInfo {
		DEFAULT,
		NONE,
		BOMB,
		FLAG,
		NUM
	};
public:
	Game(int& width, int& height) {
		SystemCoordinator::getInstance()->RegisterComponent<TransformComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<ContentComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<ScrollableComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<TextComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<TreeNodeComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<PositionComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<HoverableComponent>();
   		SystemCoordinator::getInstance()->RegisterComponent<DirtyComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<ClickableComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<RenderableIcon>();
    	SystemCoordinator::getInstance()->RegisterComponent<ParentComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<TextureComponent>();
    	SystemCoordinator::getInstance()->RegisterComponent<NonRenderableBoundingBox>();
    	SystemCoordinator::getInstance()->RegisterComponent<HandleInput>();
		SystemCoordinator::getInstance()->RegisterComponent<CellComponent>();

		iconrenderer = SystemCoordinator::getInstance()->RegisterSystem<IconRenderSystem>();
		clicksystem = SystemCoordinator::getInstance()->RegisterSystem<ClickSystem>();
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
	void revealCells(int x, int y);
	void checkCells(int x, int y);
	void setFlag(int x, int y);
};

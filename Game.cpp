#include "Game.h"
#include "Components.hpp"

void Game::Initialize() {
	for(int i = 0; i<30; i++) {
		for(int j = 0; j<16; j++) {
			cells[i][j] = SystemCoordinator::getInstance()->CreateEntity();
			RenderableIcon icon = {
				GetUV(0),
				glm::vec4(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE),
				glm::vec4(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE),
				true
			};
			SystemCoordinator::getInstance()->AddComponent(cells[i][j], icon);
			SystemCoordinator::getInstance()->AddComponent(cells[i][j], PositionComponent{glm::vec2(i,j)});
			ClickableComponent component = {glm::vec4(i*CELL_SIZE, j*CELL_SIZE, CELL_SIZE, CELL_SIZE), [this](EntityID entity) {
				
				auto& pos = SystemCoordinator::getInstance()->GetComponent<PositionComponent>(entity);
				checkCells(pos.position.x,pos.position.y);
			}};
			SystemCoordinator::getInstance()->AddComponent(cells[i][j], component);
		}
	}
}

void Game::update() {
	clicksystem->Update();
}

void Game::render() {
	iconrenderer->Update();
}

void Game::checkCells(int x, int y) {
}

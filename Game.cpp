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
			SystemCoordinator::getInstance()->AddComponent(cells[i][j], CellComponent{});
			//add non renderable bounding box here for some reason or remove from system
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

void Game::revealCells(int x, int y) {
}

void Game::setFlag(int x, int y) {

}

void Game::checkCells(int x, int y) {
	auto sc = SystemCoordinator::getInstance();
	if(Input::getInstance()->getEventType()[Input::EventType::MOUSE_UP]) {
		revealCells(x,y);
		return;
	}
	if(Input::getInstance()->getEventType()[Input::EventType::MOUSE_DRAG]) {
		auto& cell = sc->GetComponent<CellComponent>(cells[(int)lastclick.x][(int)lastclick.y]);
		sc->GetComponent<RenderableIcon>(cells[(int)lastclick.x][(int)lastclick.y]).uvRect = cell.isRevealed ? GetUV(cell.tileid) : GetUV(0);
	}
	std::cout << "test" << std::endl;
	sc->GetComponent<RenderableIcon>(cells[(int)x][(int)y]).uvRect = sc->GetComponent<CellComponent>(cells[x][y]).isRevealed ? GetUV(sc->GetComponent<CellComponent>(cells[x][y]).tileid) : GetUV(1);
	lastclick = glm::vec2(x,y);
}

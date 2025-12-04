#include "Game.h"
#include "Components.hpp"
#include <random>
#include <algorithm>

inline bool isSafe(int x, int y) {
	return x >= 0 && x < 30 && y >= 0 && y < 16;
}


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
	setUpBombs();
	for(auto& p : bombLocation) {
		int dx[8] = {1,0,-1,0,-1,1,-1,1}, dy[8] = {0,1,0,-1,-1,-1,1,1};
		for(int i = 0; i<8; i++) {
			int xx = p.first+dx[i];
			int yy = p.second+dy[i];
			if(!isSafe(xx,yy)) continue;
			auto& cell = GetComponent<CellComponent>(xx,yy);
			if(cell.tileid != 11) cell.tileid++;
		}
	}
}


void Game::setUpBombs() {
	std::vector<int> a;
	a.resize(16*30);
	fill(a.begin(),a.end(),0);
	std::random_device rdev;
	std::mt19937 rgen(rdev());

	for(int i = 0; i<BOMBCOUNT; i++) a[i] = 1;
	for(int i = a.size() -1; i>0; i--) {
		std::uniform_int_distribution<int> idist(0,i);
		int x = idist(rgen);
		std::swap(a[i],a[x]);
	}
	for(int i = 0; i<16; i++) {
		for(int j = 0; j<30; j++) { std::cout << a[i*30+j] << ' '; if(a[i*30+j]) { GetComponent<CellComponent>(j,i).tileid = 11; bombLocation.push_back({j,i}); }}
		std::cout << std::endl;
	}
	std::cout << bombLocation.size() << std::endl;
}

void Game::update() {
	clicksystem->Update();
}

void Game::render() {
	iconrenderer->Update();
}

void Game::revealCells(int x, int y) {
	auto sc = SystemCoordinator::getInstance();
	auto& cell = sc->GetComponent<CellComponent>(cells[x][y]);
	//if(cell.isRevealed || cell.isFlagged) return;
	std::queue<std::pair<int,int>> q;
	int dx[8] = {1,0,-1,0,-1,1,-1,1}, dy[8] = {0,1,0,-1,-1,-1,1,1};
	q.push({x,y});
	while(!q.empty()) {
		auto p = q.front();
		q.pop();
		for(int i = 0; i<8; i++) {
			int xx = p.first+dx[i];
			int yy = p.second+dy[i];
			if (!isSafe(xx,yy)) continue;
			auto& c = GetComponent<CellComponent>(xx,yy);
			if(c.tileid == 11 && !c.isFlagged) gameOver(xx,yy);
			if(!c.isRevealed && c.tileid != 11) {
				revealCell(xx,yy);
				if (c.tileid < 2) q.push({xx,yy});
			}
		}
	}
}

void Game::revealCell(int x, int y) {

	auto& c = GetComponent<CellComponent>(x,y);
	if (c.isRevealed) {
		int dx[8] = {1,0,-1,0,-1,1,-1,1}, dy[8] = {0,1,0,-1,-1,-1,1,1};
		int flagcnt=0;
		for(int i = 0; i<8; i++) {
			int xx = x+dx[i];
			int yy = y+dy[i];
			if(!isSafe(xx,yy)) continue;
			auto& cell = GetComponent<CellComponent>(xx,yy);
			if(cell.isFlagged) flagcnt++;
			else if (!cell.isRevealed) {
				buffer.push_back({xx,yy});
				GetComponent<RenderableIcon>(xx,yy).uvRect = GetUV(1);
			}
		}
		if(flagcnt && flagcnt == c.tileid-1) {
			revealCells(x,y);
		}
		else {
			thing();
		}
	}
	else {
		GetComponent<RenderableIcon>(x,y).uvRect = GetUV(c.tileid);
		c.isRevealed = true;
		revealedCells++;
	}

}

void Game::click(int x, int y) {
	if (GetComponent<CellComponent>(x,y).isRevealed) {
		auto& c = GetComponent<CellComponent>(x,y);
		int dx[8] = {1,0,-1,0,-1,1,-1,1}, dy[8] = {0,1,0,-1,-1,-1,1,1};
		for(int i = 0; i<8; i++) {
			int xx = x+dx[i];
			int yy = y+dy[i];
			if(!isSafe(xx,yy)) continue;
			auto& cell = GetComponent<CellComponent>(xx,yy);
			if (!cell.isFlagged && !cell.isRevealed) {
				buffer.push_back({xx,yy});
				GetComponent<RenderableIcon>(xx,yy).uvRect = GetUV(1);
			}
		}
	}
	else GetComponent<RenderableIcon>(x,y).uvRect = GetComponent<CellComponent>(x,y).isRevealed ? GetUV(GetComponent<CellComponent>(x,y).tileid) : GetUV(1);
	lastclick = glm::vec2(x,y);
}

void Game::setFlag(int x, int y) {
	auto sc = SystemCoordinator::getInstance();
	auto& cell = SystemCoordinator::getInstance()->GetComponent<CellComponent>(cells[x][y]);
	if(cell.isRevealed) return;
	cell.isFlagged = !cell.isFlagged;
	flaggedLocations[{x,y}] = cell.isFlagged;
	sc->GetComponent<RenderableIcon>(cells[x][y]).uvRect = GetUV(10*cell.isFlagged);
}

void Game::gameOver(int x, int y) {
	for (auto& p : bombLocation) {
		auto& c = GetComponent<CellComponent>(p.first,p.second);
		if (!c.isFlagged) GetComponent<RenderableIcon>(p.first,p.second).uvRect = GetUV(c.tileid);
	}
	GetComponent<RenderableIcon>(x,y).uvRect = GetUV(12);
	for (auto& p : flaggedLocations) {
		if (p.second && std::find(bombLocation.begin(), bombLocation.end(), p.first) == bombLocation.end()) GetComponent<RenderableIcon>(p.first.first, p.first.second).uvRect = GetUV(13);
	}
}

void Game::checkCells(int x, int y) {
	auto sc = SystemCoordinator::getInstance();
	if(Input::getInstance()->getEventType()[Input::EventType::MOUSE_UP]) {
		//auto& cell = sc->GetComponent<CellComponent>(cells[(int)lastclick.x][(int)lastclick.y]);
		//sc->GetComponent<RenderableIcon>(cells[(int)lastclick.x][(int)lastclick.y]).uvRect = cell.isRevealed ? GetUV(cell.tileid) : GetUV(0);
		if (Input::getInstance()->getMouseType()) return;
		if (GetComponent<CellComponent>(x,y).tileid == 1) revealCells(x,y);
		else if (GetComponent<CellComponent>(x,y).tileid == 11) gameOver(x,y);
		else revealCell(x,y);
		return;
	}
	if(Input::getInstance()->getEventType()[Input::EventType::MOUSE_DRAG]) {
		auto& cell = sc->GetComponent<CellComponent>(cells[(int)lastclick.x][(int)lastclick.y]);
		sc->GetComponent<RenderableIcon>(cells[(int)lastclick.x][(int)lastclick.y]).uvRect = cell.isRevealed ? GetUV(cell.tileid) : GetUV(0);
		thing();
	}
	if(Input::getInstance()->getMouseType())  {
		setFlag(x,y);
		return;
	}
	if(sc->GetComponent<CellComponent>(cells[x][y]).isFlagged) return;
	click(x,y);
}

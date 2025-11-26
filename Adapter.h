// #pragma once
// #include "header.h"
// #include "EventHandler.h"
//
//
//
// class Adapter {
// protected:
// 	EventHandler* eventHandler;
// public:
// 	Adapter() {
// 		eventHandler = new EventHandler();
// 		eventHandler->registerEvent(new Event<glm::vec2>("click", std::function<void(glm::vec2)>(std::bind(&Adapter::onClick, this, std::placeholders::_1))));
// 		eventHandler->registerEvent(new Event<glm::vec2,glm::vec2>("drag", std::function<void(glm::vec2,glm::vec2)>(std::bind(&Adapter::onDrag, this, std::placeholders::_1, std::placeholders::_2))));
// 		eventHandler->registerEvent(new Event<glm::vec2>("release", std::function<void(glm::vec2)>(std::bind(&Adapter::onRelease, this, std::placeholders::_1))));
// 		eventHandler->registerEvent(new Event<glm::vec2>("doubleclick", std::function<void(glm::vec2)>(std::bind(&Adapter::onDoubleClick, this, std::placeholders::_1))));
// 		eventHandler->registerEvent(new Event<int,int>("update", std::function<void(int,int)>(std::bind(&Adapter::onUpdate, this, std::placeholders::_1, std::placeholders::_2))));
// 	};
// 	~Adapter() {
// 		delete eventHandler;
// 	};
// 	virtual void onClick(glm::vec2 pos) = 0;
// 	virtual void onDrag(glm::vec2 pos, glm::vec2 pos2) = 0;
// 	virtual void onRelease(glm::vec2 pos) = 0;
// 	virtual void onDoubleClick(glm::vec2 pos) = 0;
// 	virtual void onUpdate(int start, int end) = 0;
// 	template<typename ...Args>
// 	void queueEvent(const std::string& id, Args... args) {
// 		eventHandler->queueEvent(id, args...);
// 	}
// 	void dispatchEvent() {
// 		eventHandler->dispatchEvent();
// 	}
// 	void dispatchUpdate() {
// 		eventHandler->dispatchUpdate();
// 	}
// 	template<typename ...Args>
// 	void callEvent(const std::string& id, Args... args) {
// 		eventHandler->callEvent(id, args...);
// 	}
// };
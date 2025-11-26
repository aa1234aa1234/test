// #pragma once
// #include "header.h"
// #include <functional>
// #include <typeindex>
// #include <queue>
// #include <map>
// #include <any>
//
// template<typename ...Args>
// class Event;
//
// class IEvent {
// public:
// 	virtual const int getId() = 0;
// 	virtual std::type_index getType() = 0;
// 	virtual ~IEvent() = default;
// 	// template<typename ...Args>
// 	// Event<Args...>* get() {
// 	// 	return dynamic_cast<Event<Args...>*>(this);
// 	// }
// 	template<class T>
// 	T getAs()
// 	{
// 		return static_cast<T>(this);
// 	}
// };
//
// // template<typename ...Args>
// // class Event : public IEvent {
// // 	std::string eventId;
// // 	std::function<void(Args...)> const func;
// // public:
// // 	Event(const std::string& id, std::function<void(Args...)> func) : eventId(id), func(func) {
// //
// // 	}
// // 	~Event() {}
// //
// // 	const std::string& getId() override { return eventId; }
// // 	std::type_index getType() override {
// // 		return typeid(Event<Args...>);
// // 	}
// // 	void callEvent(Args... args) {
// // 		this->func(args...);
// // 	}
// // };
//
// class EventListener
// {
// public:
// 	EventListener() {}
// 	virtual ~EventListener() = default;
// 	virtual void handleEvent(IEvent& event);
// };
//
// class EventDispatcher
// {
// 	std::vector<EventListener*> listeners;
// 	std::vector<std::function<void(IEvent&)>> callbacks;
// public:
// 	EventDispatcher() {}
// 	virtual ~EventDispatcher() = default;
//
// 	void registerCallback(std::function<void(IEvent&)> callback)
// 	{
// 		callbacks.push_back(callback);
// 	}
//
// 	void registerListener(EventListener* listener)
// 	{
// 		listeners.push_back(listener);
// 	}
//
// 	void dispatchEvent(IEvent& event)
// 	{
// 		for (auto& p : listeners)
// 		{
// 			p->handleEvent(event);
// 		}
//
// 		for (auto& p : callbacks)
// 		{
// 			p(event);
// 		}
// 	}
// };
//
// // class EventHandler
// // {
// // 	std::map<std::string, std::vector<IEvent*>> eventList;
// // 	std::queue<std::function<void()>> eventQueue, updateQueue;
// //
// //
// //
// // public:
// //
// // 	static EventHandler& Ref() {
// // 		static EventHandler ref;
// // 		return ref;
// // 	}
// //
// // 	EventHandler() {}
// // 	~EventHandler() {
// // 		for (auto& k : eventList) {
// // 			for (auto& p : k.second) {
// // 				delete p;
// // 			}
// // 			k.second.clear();
// // 		}
// // 	}
// //
// // 	void registerEvent(IEvent* event) {
// // 		eventList[event->getId()].push_back(event);
// // 	}
// //
// // 	template<typename ...Args>
// // 	void callEvent(const std::string& id, Args... args) {
// // 		for (auto& p : eventList[id]) {
// //
// // 			Event<Args...>* event = p->get<Args...>();
// // 			if (event == nullptr) {
// // 				std::cout << "Registered type: " << p->getType().name() << ", Requested: " << typeid(Event<Args...>).name() << std::endl;
// // 				std::cout << "nullptr" << std::endl;
// // 				return;
// // 			}
// // 			event->callEvent(args...);
// // 		}
// // 	}
// //
// // 	template<typename ...Args>
// // 	void queueEvent(const std::string& id, Args... args) {
// // 		if (id != "update") eventQueue.push([this, id, args...]() { this->callEvent<Args...>(id, args...); });
// // 		else updateQueue.push([this, id, args...]() { this->callEvent<Args...>(id, args...); });
// // 	}
// //
// // 	void dispatchEvent() {
// // 		if (eventQueue.size()) {
// // 			eventQueue.front()();
// // 			eventQueue.pop();
// // 		}
// // 	}
// //
// // 	void dispatchUpdate() {
// // 		if (updateQueue.size()) {
// // 			updateQueue.front()();
// // 			updateQueue.pop();
// // 		}
// // 	}
// // };
//
// //static EventHandler* eventHandler = &EventHandler::Ref();
//

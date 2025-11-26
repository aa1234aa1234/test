//
// Created by D412 on 2025-07-07.
//
#pragma once
#ifndef EVENTDISPATCHER_H
#pragma once
#include "header.h"
#include <functional>
#include <typeindex>
#include <queue>
#include <map>
#include <any>


class IEvent {
public:
	virtual const int getId() = 0;
	virtual std::type_index getType() = 0;
	virtual ~IEvent() = default;
	// template<typename ...Args>
	// Event<Args...>* get() {
	// 	return dynamic_cast<Event<Args...>*>(this);
	// }
	template<class T>
	T getAs()
	{
		return static_cast<T>(this);
	}
};


class EventListener
{
public:
	EventListener() {}
	virtual ~EventListener() = default;
	virtual void handleEvent(IEvent& event);
};

class EventDispatcher
{
	std::vector<EventListener*> listeners;
	std::vector<std::function<void(IEvent&)>> callbacks;
public:
	EventDispatcher() {}
	virtual ~EventDispatcher() = default;

	void registerCallback(std::function<void(IEvent&)> callback)
	{
		callbacks.push_back(callback);
	}

	void registerListener(EventListener* listener)
	{
		listeners.push_back(listener);
	}

	void dispatchEvent(IEvent& event)
	{
		for (auto& p : listeners)
		{
			p->handleEvent(event);
		}

		for (auto& p : callbacks)
		{
			p(event);
		}
	}
};




#define EVENTDISPATCHER_H

#endif //EVENTDISPATCHER_H

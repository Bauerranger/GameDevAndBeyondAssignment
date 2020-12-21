#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <functional>

class IEvent
{
public:
	virtual ~IEvent()
	{

	}
};

typedef std::function<void(std::shared_ptr<IEvent>)> EventFunctor;

class IEventHandler
{
public:
	virtual ~IEventHandler()
	{

	}
public:
	virtual bool DoesEventMatch(std::shared_ptr<IEvent> event) = 0;
	void AddCallback(EventFunctor& callback);
	void RemoveCallback(EventFunctor& callback);
	void Call(std::shared_ptr<IEvent> event);
private:
	std::vector<EventFunctor> m_Callbacks;
};

class EventManager
{
public:
	EventManager();
	~EventManager();

	static EventManager& GetInstance()
	{
		static EventManager _instance;
		return _instance;
	}

	EventManager(const EventManager&) = delete; //delete copy constructor
	EventManager& operator=(const EventManager&) = delete; //delete copy operator

public:
	void AddEventListener(std::shared_ptr<IEventHandler> listener);
	void RemoveEventListener(std::shared_ptr<IEventHandler> listener);
	void PushEvent(std::shared_ptr<IEvent> event);
	void Update();

private:
	std::vector<std::shared_ptr<IEventHandler>> m_Listeners;
	std::queue<std::shared_ptr<IEvent>> m_Events;
};
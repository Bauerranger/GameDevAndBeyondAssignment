#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <SFMLEngine/Header Files/Engine.h>

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
	void SetSystemThread(eThreadImportance usedThread) { m_SystemThread = usedThread; };
	void GetSystemThread(eThreadImportance& usedThread) { usedThread = m_SystemThread; };
private:
	std::vector<EventFunctor> m_Callbacks;
	eThreadImportance m_SystemThread;
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
	void AddEventListener(std::shared_ptr<IEventHandler> listener, eThreadImportance usedThread);
	void RemoveEventListener(std::shared_ptr<IEventHandler> listener);
	void PushEvent(std::shared_ptr<IEvent> event);
	void Update(eThreadImportance usedThread);

private:
	std::vector<std::shared_ptr<IEventHandler>> m_Listeners;
	std::queue<std::shared_ptr<IEvent>> m_MainEvents;
	std::queue<std::shared_ptr<IEvent>> m_WorkerEvents;
	std::queue<std::shared_ptr<IEvent>> m_RenderEvents;
};
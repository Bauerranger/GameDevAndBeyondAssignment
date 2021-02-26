#include "../Header Files/EventManager.h"
#include "GameDevAndBeyond/Header Files/Events.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{

}

void EventManager::AddEventListener(std::shared_ptr<IEventHandler> listener, eThreadImportance usedThread)
{
	if (std::find(m_Listeners.begin(), m_Listeners.end(), listener) != m_Listeners.end())
	{
		return;
	}
	listener->SetSystemThread(usedThread);
	m_Listeners.push_back(listener);
}

void EventManager::RemoveEventListener(std::shared_ptr<IEventHandler> listener)
{
	std::vector<std::shared_ptr<IEventHandler>>::iterator listenerIterator = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
	if (listenerIterator == m_Listeners.end())
	{
		return;
	}
	m_Listeners.erase(listenerIterator);
}

void EventManager::PushEvent(std::shared_ptr<IEvent> event)
{
	m_MainEvents.push(event);
	m_WorkerEvents.push(event);
	m_RenderEvents.push(event);
}

void EventManager::Update(eThreadImportance usedThread)
{
	std::queue<std::shared_ptr<IEvent>> events;
	switch (usedThread)
	{
	case eThreadImportance::direct:
		events = m_MainEvents;
		break;
	case eThreadImportance::worker:
		events = m_WorkerEvents;
		break;
	case eThreadImportance::render:
		events = m_RenderEvents;
		break;
	default:
		events = m_MainEvents;
		break;
	}
	while (!events.empty())
	{
		std::shared_ptr<IEvent> event = events.front();
		events.pop();
		std::vector<std::shared_ptr<IEventHandler>> copiedListeners = m_Listeners;
		for (std::shared_ptr<IEventHandler> listener : m_Listeners)
		{
			eThreadImportance importance;
			listener->GetSystemThread(importance);
			if (usedThread == importance && listener->DoesEventMatch(event))
			{
				listener->Call(event);
			}
		}
	}
}

void IEventHandler::AddCallback(EventFunctor& callback)
{
	auto found = std::find_if(m_Callbacks.begin(), m_Callbacks.end(), [&](const EventFunctor& foundCallback) -> bool
		{
			return callback.target<void(std::shared_ptr<IEvent>)>() == foundCallback.target<void(std::shared_ptr<IEvent>)>()
				&& callback.target_type() == foundCallback.target_type();
		});
	if (found != m_Callbacks.end())
	{
		return;
	}
	m_Callbacks.push_back(callback);
}

void IEventHandler::RemoveCallback(EventFunctor& callback)
{
	std::vector<EventFunctor>::iterator found = std::find_if(m_Callbacks.begin(), m_Callbacks.end(), [&](const EventFunctor& foundCallback) -> bool
		{
			return callback.target<void(std::shared_ptr<IEvent>)>() == foundCallback.target<void(std::shared_ptr<IEvent>)>();
		});
	if (found == m_Callbacks.end())
	{
		return;
	}
	m_Callbacks.erase(found);
}

void IEventHandler::Call(std::shared_ptr<IEvent> event)
{
	for (EventFunctor& callback : m_Callbacks)
	{
		callback(event);
	}
}

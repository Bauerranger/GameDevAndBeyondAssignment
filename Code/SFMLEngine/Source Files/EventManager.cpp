#include "../Header Files/EventManager.h"
#include "GameDevAndBeyond/Header Files/Events.h"

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

///////////////////////////////////////////////////// Logic

void EventManager::AddEventListener(std::shared_ptr<IEventHandler> listener)
{
	if (std::find(m_Listeners.begin(), m_Listeners.end(), listener) != m_Listeners.end())
	{
		return;
	}
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
	m_Events.push(event);
}

void EventManager::Update()
{
	while (!m_Events.empty())
	{
		std::shared_ptr<IEvent> event = m_Events.front();
		m_Events.pop();

		std::vector<std::shared_ptr<IEventHandler>> copiedListeners = m_Listeners;
		for (const std::shared_ptr<IEventHandler>& listener : m_Listeners)
		{
			if (listener->DoesEventMatch(event))
			{
				listener->Call(event);
			}
		}
	}
}

void IEventHandler::AddCallback(EventFunctor& callback)
{
	// find callback if lambda confirmed that it is of same event && right type
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
	// return callback if lambda confirmed that it is of same event && right type
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

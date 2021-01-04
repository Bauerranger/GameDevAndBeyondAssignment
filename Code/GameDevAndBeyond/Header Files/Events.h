#pragma once
#include <SFMLEngine/Header Files/EventManager.h>	

class SoundSpawnedEvent : public IEvent {};

class ScoreEvent : public IEvent
{
public:
	int Score = 0;
};

class EventHandler : public IEventHandler
{
public:
	bool DoesEventMatch(std::shared_ptr<IEvent> event)
	{
		if (std::dynamic_pointer_cast<SoundSpawnedEvent>(event) != nullptr)
		{
			return true;
		}
		return false;
	}
};
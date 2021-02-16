#pragma once
#include <SFMLEngine/Header Files/EventManager.h>	
#include <string>

class SoundSpawnEvent : public IEvent 
{
public:
	std::string FilePath;
	bool IsLooping;
};

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
		if (std::dynamic_pointer_cast<SoundSpawnEvent>(event) != nullptr)
		{
			return true;
		}
		return false;
	}
};
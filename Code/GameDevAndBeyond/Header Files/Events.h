#pragma once
#include <SFMLEngine/Header Files/EventManager.h>	

class CollisionEvent : public IEvent
{
};

class SpawnEvent : public IEvent
{
};

// TODO: make sound event

class PhysicUpdateEvent : public IEvent
{
};

class LooseEvent : public IEvent
{
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
		if (std::dynamic_pointer_cast<ScoreEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<SpawnEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<CollisionEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<PhysicUpdateEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<LooseEvent>(event) != nullptr)
		{
			return true;
		}
		return false;
	}
};
#pragma once
#include <SFMLEngine/Header Files/EventManager.h>	

class GameStartEvent : public IEvent
{
};

class SpawnEvent : public IEvent
{
};

class PhysicUpdateEvent : public IEvent
{
};

class CollisionEvent : public IEvent
{
};

class LevelUpEvent : public IEvent
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

class GameStateChangeEvent : public IEvent
{
};

class GameRestartEvent : public IEvent
{
};

class EventHandler : public IEventHandler
{
public:
	bool DoesEventMatch(std::shared_ptr<IEvent> event) override
	{
		if (std::dynamic_pointer_cast<GameStartEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<SpawnEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<PhysicUpdateEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<CollisionEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<LevelUpEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<LooseEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<ScoreEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<GameRestartEvent>(event) != nullptr
			|| std::dynamic_pointer_cast<GameStateChangeEvent>(event) != nullptr)
		{
			return true;
		}
		return false;
	}
};
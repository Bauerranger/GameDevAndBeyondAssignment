#pragma once

#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include "Events.h"

class MapSystem : public ISystem
{
public:
	MapSystem();
	~MapSystem();

	///////////////////////////////////////////////////// ISystem
public:
	void Init(Engine* engine) override;
	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	void Update(Engine* engine, float dt) override;
	void UpdateMap();

	///////////////////////////////////////////////////// Logic
private:
	void UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt);
	void UpdateSingleEntityCollision(std::shared_ptr<Entity> entity, float dt);

	///////////////////////////////////////////////////// Events
	void OnEvent(std::shared_ptr<IEvent> event);
	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_EventFunctor;

	///////////////////////////////////////////////////// Utility
	float m_DeltaTime;

	///////////////////////////////////////////////////// Map
	std::atomic_bool m_MapMatrix[20][10] = { false };
	std::shared_ptr<Entity> m_BGEntity;

	std::atomic_bool m_CollisionHasHappened = false;
};
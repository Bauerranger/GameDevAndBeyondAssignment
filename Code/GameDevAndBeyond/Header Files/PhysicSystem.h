#pragma once
#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include "Events.h"

class PhysicSystem : public ISystem
{
public:
	PhysicSystem();
	~PhysicSystem();
	//ISystem
public:
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	virtual void Update(Engine* engine, float dt) override;
	
private:
	void inline UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt);
	void OnCollision(std::shared_ptr<IEvent> event);
	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_CollisionEventFunctor;
	float m_TickTime = 0;
	float m_TickWaitTime = 0;
};
#pragma once

#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include "Events.h"

class MapSystem : public ISystem
{
public:
	MapSystem();
	~MapSystem();
public:
	void Init(Engine* engine) override;
	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	void Update(Engine* engine, float dt) override;
	void UpdateMap();

private:
	void LoadUI(Engine* engine);
	void UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt);
	void UpdateSingleEntityCollision(std::shared_ptr<Entity> entity, float dt);
	void OnPhysicsUpdate(std::shared_ptr<IEvent> event);
	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_PhysicsUpdateEventFunctor;
	float m_DeltaTime;
	std::atomic_bool m_MapMatrix[20][10] = { 0 };
	std::atomic_bool m_CollisionHasHappened = false;
	Engine* m_Engine;
	std::vector<std::shared_ptr<Entity>> m_MarkedForDelete;
};
#include "../Header Files/PhysicSystem.h"
#include "../Header Files/PhysicComponent.h"
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <SFMLEngine/Header Files/Engine.h>
#include <cmath>
#include <iostream>

PhysicSystem::PhysicSystem()
{
	// Add events
	m_Listener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&PhysicSystem::OnEvent, this, std::placeholders::_1);

	m_Listener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_Listener);
}

PhysicSystem::~PhysicSystem()
{
	// Remove events
	EventManager::GetInstance().RemoveEventListener(m_Listener);
	m_Listener->RemoveCallback(m_EventFunctor);
}

////////////////////////////////////////////////////////////////////////////////////// ISystem

bool PhysicSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<PhysicComponent>())
	{
		return true;
	}
	return false;
}

void PhysicSystem::Update(Engine * engine, float dt)
{
	if(!Engine::Instance && !Engine::Instance()->IsRunning())
	{
		return;
	}

	// Only update when game is active
	eGameState state;
	Engine::Instance()->GetGameState(state);
	if (state != eGameState::game) 
	{
		return;
	}

	m_TickTime += dt;
	if (m_TickTime > m_TickWaitTime) {
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;

		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			// Tick single entity if it is falling
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				UpdateSingleEntityPosition(entity, dt);
			}
			else 
			{
				RemoveEntity(entity);
			}

			++entityItr;
		}

		// Start other physic event based functions
		std::shared_ptr<PhysicUpdateEvent> physicEvent = std::make_shared<PhysicUpdateEvent>();
		EventManager::GetInstance().PushEvent(physicEvent);
		m_TickTime = 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////// Entity logic

inline void PhysicSystem::UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt)
{
	// Set sprite position one lower
	float spritePositionX = 0;
	float spritePositionY = 0;

	entity->GetComponent<SpriteComponent>()->GetPosition(spritePositionX, spritePositionY);

	entity->GetComponent<SpriteComponent>()->SetPosition(spritePositionX, spritePositionY + SPRITESIZE);

	// reset wait time if the block was dropped
	float waitTime = 0;
	entity->GetComponent<PhysicComponent>()->GetUpdateWaitTime(waitTime);
	m_TickWaitTime = waitTime;
}

////////////////////////////////////////////////////////////////////////////////////// Events

void PhysicSystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<CollisionEvent> collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
	if (collisionEvent != nullptr)
	{
		// Stop entities from falling on collision
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			entity->RemoveComponent<PhysicComponent>();

			m_TickWaitTime = m_CurrentWaitTime;

			++entityItr;
		}
		std::shared_ptr<SpawnEvent> spawnEvent = std::make_shared<SpawnEvent>();
		EventManager::GetInstance().PushEvent(spawnEvent);
	}

	std::shared_ptr<LevelUpEvent> levelUpEvent = std::dynamic_pointer_cast<LevelUpEvent>(event);
	if (levelUpEvent != nullptr)
	{
		// Make blocks fall faster
		m_CurrentWaitTime -= 0.03;
		m_TickWaitTime = m_CurrentWaitTime;
	}
}



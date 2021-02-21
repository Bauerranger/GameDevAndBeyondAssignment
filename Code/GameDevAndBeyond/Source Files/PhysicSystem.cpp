#include "../Header Files/PhysicSystem.h"
#include "../Header Files/PhysicComponent.h"
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <SFMLEngine/Header Files/Engine.h>
#include <cmath>
#include <iostream>

PhysicSystem::PhysicSystem()
{
	m_Listener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&PhysicSystem::OnEvent, this, std::placeholders::_1);
	m_Listener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_Listener);
}

PhysicSystem::~PhysicSystem()
{
	EventManager::GetInstance().RemoveEventListener(m_Listener);
	m_Listener->RemoveCallback(m_EventFunctor);
}

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
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	
	m_TickTime += dt;
	if (m_TickTime > m_TickWaitTime) {
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
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
		std::shared_ptr<PhysicUpdateEvent> physicEvent = std::make_shared<PhysicUpdateEvent>();
		EventManager::GetInstance().PushEvent(physicEvent);
		m_TickTime = 0;
	}
}

inline void PhysicSystem::UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt)
{
	float spritePositionX = 0;
	float spritePositionY = 0;

	entity->GetComponent<SpriteComponent>()->GetPosition(spritePositionX, spritePositionY);
	entity->GetComponent<SpriteComponent>()->SetPosition(spritePositionX, spritePositionY + 45);
	float waitTime = 0;
	entity->GetComponent<PhysicComponent>()->GetUpdateWaitTime(waitTime);
	m_TickWaitTime = waitTime;
}

void PhysicSystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<CollisionEvent> collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
	if (collisionEvent != nullptr)
	{
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			entity->RemoveComponent<PhysicComponent>();
			++entityItr;

			float waitTime = 0.5f;
			m_TickWaitTime = waitTime;
		}
		std::shared_ptr<SpawnEvent> spawnEvent = std::make_shared<SpawnEvent>();
		EventManager::GetInstance().PushEvent(spawnEvent);
	}
}



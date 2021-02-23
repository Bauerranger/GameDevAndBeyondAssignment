#include "SFMLEngine/Header Files/AudioSystem.h"
#include <iostream>
#include <SFMLEngine/Header Files/Engine.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include <filesystem>

AudioSystem::AudioSystem()
= default;

AudioSystem::~AudioSystem() 
{
	m_Listener->RemoveCallback(m_EventFunctor);
	EventManager::GetInstance().RemoveEventListener(m_Listener);
}

void AudioSystem::AddEntity(std::shared_ptr<Entity> entity)
{
	ISystem::AddEntity(entity);
	entity->GetComponent<AudioComponent>()->PlayAudio();
}

bool AudioSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<AudioComponent>())
	{
		return true;
	}
	return false;
}

void AudioSystem::Update(Engine* engine, float dt)
{
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		std::shared_ptr<Entity> entity = *entityItr;
		if (!UpdateSingleEntity(engine, entity, dt))
		{
			engine->RemoveEntity(entity);
		}
		++entityItr;
	}
}

void AudioSystem::Init(Engine* engine) 
{
	m_Listener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&AudioSystem::OnEvent, this, std::placeholders::_1);
	m_Listener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_Listener);
}

inline bool AudioSystem::UpdateSingleEntity(Engine* engine, std::shared_ptr<Entity> entity, float dt)
{
	std::shared_ptr<AudioComponent> component;
	component = entity->GetComponent<AudioComponent>();
	return !component->AudioIsFinished();
}

void AudioSystem::PlayMusic()
{
	if (!Engine::Instance)
	{
		return;
	}
	if (std::filesystem::exists("../bin/Tetris_theme.ogg"))
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>();
		std::shared_ptr<AudioComponent> comp = entity->AddComponent("../bin/Tetris_theme.ogg", true);
		Engine::Instance()->AddEntity(entity);
	}
}

void AudioSystem::PlayScore()
{
	if (!Engine::Instance)
	{
		return;
	}
	if (std::filesystem::exists("../bin/Score.ogg"))
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>();
		std::shared_ptr<AudioComponent> comp = entity->AddComponent("../bin/Score.ogg", false);
		Engine::Instance()->AddEntity(entity);
	}
}

void AudioSystem::PlayCollision()
{
	if (!Engine::Instance)
	{
		return;
	}
	if (std::filesystem::exists("../bin/Collision.ogg"))
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>();
		std::shared_ptr<AudioComponent> comp = entity->AddComponent("../bin/Collision.ogg", false);
		Engine::Instance()->AddEntity(entity);
	}
}

void AudioSystem::PlayEnd()
{
	if (!Engine::Instance)
	{
		return;
	}
	if (std::filesystem::exists("../bin/End.ogg"))
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>();
		std::shared_ptr<AudioComponent> comp = entity->AddComponent("../bin/End.ogg", false);
		Engine::Instance()->AddEntity(entity);
	}
}

void AudioSystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<GameStartEvent> startEvent = std::dynamic_pointer_cast<GameStartEvent>(event);
	std::shared_ptr<CollisionEvent> collisionEvent = std::dynamic_pointer_cast<CollisionEvent>(event);
	std::shared_ptr<ScoreEvent> scoreEvent = std::dynamic_pointer_cast<ScoreEvent>(event);
	std::shared_ptr<LooseEvent> looseEvent = std::dynamic_pointer_cast<LooseEvent>(event);


	if (startEvent != nullptr)
	{
		PlayMusic();
	}

	if (collisionEvent != nullptr)
	{
		PlayCollision();
	}

	if (scoreEvent != nullptr)
	{
		PlayScore();
	}

	if (looseEvent != nullptr)
	{
		PlayEnd();
	}
}

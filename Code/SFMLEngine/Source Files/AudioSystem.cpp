#include "SFMLEngine/Header Files/AudioSystem.h"
#include <iostream>
#include <SFMLEngine/Header Files/Engine.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include <filesystem>

AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
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
	if (std::filesystem::exists("../bin/Tetris_theme.ogg"))
	{
		std::shared_ptr<Entity> entity = std::make_shared<Entity>();
		std::shared_ptr<AudioComponent> comp = entity->AddComponent("../bin/Tetris_theme.ogg", true);
		engine->AddEntity(entity);
	}
}

inline bool AudioSystem::UpdateSingleEntity(Engine* engine, std::shared_ptr<Entity> entity, float dt)
{
	std::shared_ptr<AudioComponent> component;
	component = entity->GetComponent<AudioComponent>();
	return !component->AudioIsFinished();
}

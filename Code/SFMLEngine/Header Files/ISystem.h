#pragma once

#include <memory>
#include <mutex>
#include "Entity.h"

#define SPRITESIZE 45

class Engine;

class ISystem
{
public:
	///////////////////////////////////////////////////// Initialization and update

	virtual void Init(Engine* engine) = 0;
	virtual void Update(Engine* engine, float dt) = 0;

	///////////////////////////////////////////////////// Entities

	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) = 0;
	virtual void AddEntity(std::shared_ptr<Entity> entity);
	void RemoveEntity(std::shared_ptr<Entity> entity);

protected:
	std::vector<std::shared_ptr<Entity>> m_Entities;

	///////////////////////////////////////////////////// Multi threading

	std::mutex m_Mutex;

};
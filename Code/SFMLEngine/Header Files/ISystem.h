#pragma once

#include <memory>
#include "Entity.h"

class Engine;

class ISystem
{
public:
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) = 0;
	virtual void Update(Engine* engine, float dt);
	virtual void Init(Engine* engine);
	virtual void AddEntity(std::shared_ptr<Entity> entity);
	void RemoveEntity(std::shared_ptr<Entity> entity);

protected:
	std::vector<std::shared_ptr<Entity>> m_Entities;
};
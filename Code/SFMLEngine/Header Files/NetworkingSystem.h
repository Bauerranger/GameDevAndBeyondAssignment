#pragma once

#include "ISystem.h"

class NetworkingSystem : public ISystem
{
public:
	NetworkingSystem();
	~NetworkingSystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;

public:
};
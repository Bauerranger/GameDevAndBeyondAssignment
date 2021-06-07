#pragma once

#include "ISystem.h"

// Unused

class NetworkingSystem : public ISystem
{
public:
	NetworkingSystem();
	~NetworkingSystem();
public:
	//ISystem
	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;

public:
};
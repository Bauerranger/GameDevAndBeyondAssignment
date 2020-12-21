#pragma once

#include "ISystem.h"
#include "Window.h"

class RenderSystem : public ISystem
{
public:
	RenderSystem();
	~RenderSystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;

public:
	void Draw(std::shared_ptr<Window> window);
};
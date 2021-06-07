#pragma once

#include "ISystem.h"
#include "Window.h"

class RenderSystem : public ISystem
{
public:
	RenderSystem();
	~RenderSystem();
public:
	///////////////////////////////////ISystem
	
	void Init(Engine* engine) override {}; // unused
	void Update(Engine* engine, float dt) override {}; // unused

	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;

public:

	///////////////////////////////////Render
	
	void Draw(std::shared_ptr<Window> window);
};
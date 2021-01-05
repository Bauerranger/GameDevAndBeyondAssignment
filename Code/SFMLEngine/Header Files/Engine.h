#pragma once

#include <string>
#include <vector>
#include <memory>
#include <thread>

#include "Window.h"
#include "InputHelper.h"
#include "Time.h"

#include "Entity.h"
#include "ISystem.h"
#include "RenderSystem.h"

enum class eThreadImportance : std::int16_t
{
	direct,
	lazy,
	render
};

class Engine
{
public:
	Engine(int width, int height, std::string text, bool fullscreen);
	~Engine();

public:
	//Window handling
	const bool IsRunning() const { return m_IsRunning; }
	void Update();

	void UpdateLazySystems();
	void UpdateRenderSystems();

	//Rendering
	void Draw();

	//Input handling
	bool IsMousePressed(MouseButton button);
	bool IsKeyPressed(Key key);
	void GetCursorPosition(float& X, float& Y) const;

	//Time handling
	const float GetElapsedTimeAsSeconds();

	//ECS
	void AddEntity(std::shared_ptr<Entity> entity);
	void RemoveEntity(std::shared_ptr<Entity> entity);
	void AddSystem(std::shared_ptr<ISystem> system, eThreadImportance importance);
	void RemoveSystem(std::shared_ptr<ISystem> entity);

	//Window
	const std::shared_ptr<Window> GetWindow() const;
	void SetViewCenter(float X, float Y);
	void GetViewCenter(float& X, float& Y);

private:
	//Window handling
	float m_AccumulatedTime = 0.f;
	float m_AccumulatedTimeLazy = 0.f;
	float m_AccumulatedTimeRender = 0.f;
	bool m_IsRunning;
	std::shared_ptr<Window> m_Window;
	//Rendering
	std::shared_ptr<RenderSystem> m_RenderSystem;
	//Time handling
	std::unique_ptr<Time> m_Time;
	//Entities
	std::vector <std::shared_ptr<Entity>> m_Entities;
	//Systems
	std::vector <std::shared_ptr<ISystem>> m_DirectSystems;
	std::vector <std::shared_ptr<ISystem>> m_LazySystems;
	std::vector <std::shared_ptr<ISystem>> m_RenderSystems;

	const float m_dt = 1/60.f;

	// Threading
	std::thread m_LazyThread;
	std::thread m_RenderThread;
};
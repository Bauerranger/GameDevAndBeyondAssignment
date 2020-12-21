#include "../Header Files/Engine.h"
#include "../Header Files/TextureManager.h"
#include "../Header Files/EventManager.h"
#include <algorithm>

Engine::Engine(int width, int height, std::string text, bool fullscreen)
{
	m_IsRunning = true;
	m_Window = std::make_shared<Window>(width, height, text, fullscreen);
	m_Time = std::make_unique<Time>();
	m_RenderSystem = std::make_shared<RenderSystem>();
	AddSystem(m_RenderSystem);
}

Engine::~Engine()
{
	m_IsRunning = false;
}

void Engine::Update()
{
	if (!m_Window->Update())
	{
		//Window has been closed
		m_IsRunning = false;
		return;
	}

	m_Time->Restart();

	const float dt = 1 / 60.0f;

	const float elapsedTime = GetElapsedTimeAsSeconds();

	m_AccumulatedTime += elapsedTime;
	while (m_AccumulatedTime >= dt)
	{
		//update all systems
		for (std::shared_ptr<ISystem> system : m_Systems)
		{
			system->Update(this, dt);
		}
		//update all events
		EventManager::GetInstance().Update();
		//at the end of the update, decrement the accumulator by the fixed time
		m_AccumulatedTime -= dt;
	}
}

void Engine::Draw()
{
	m_Window->BeginDraw();
	m_RenderSystem->Draw(m_Window);
	m_Window->EndDraw();
}

bool Engine::IsMousePressed(MouseButton button)
{
	return InputHelper::IsMousePressed(button);
}

bool Engine::IsKeyPressed(Key key)
{
	return InputHelper::IsKeyPressed(key);
}

void Engine::GetCursorPosition(float& X, float& Y) const
{
	InputHelper::GetCursorPosition(this, X, Y);
}

const float Engine::GetElapsedTimeAsSeconds()
{
	return m_Time->GetElapsedTimeAsSeconds();
}

void Engine::AddEntity(std::shared_ptr<Entity> entity)
{
	if (std::find(m_Entities.begin(), m_Entities.end(), entity) != m_Entities.end())
	{
		return;
	}
	m_Entities.push_back(entity);

	for (std::shared_ptr<ISystem> system : m_Systems)
	{
		if (system->DoesEntityMatch(entity))
		{
			system->AddEntity(entity);
		}
	}
}

void Engine::RemoveEntity(std::shared_ptr<Entity> entity)
{
	std::vector<std::shared_ptr<Entity>>::iterator entityIterator = std::find(m_Entities.begin(), m_Entities.end(), entity);
	if (entityIterator == m_Entities.end())
	{
		return;
	}
	m_Entities.erase(entityIterator);

	for (std::shared_ptr<ISystem> system : m_Systems)
	{
		system->RemoveEntity(entity);
	}
}

void Engine::AddSystem(std::shared_ptr<ISystem> system)
{
	if (std::find(m_Systems.begin(), m_Systems.end(), system) != m_Systems.end())
	{
		return;
	}
	m_Systems.push_back(system);

	for (std::shared_ptr<Entity> entity : m_Entities)
	{
		if (system->DoesEntityMatch(entity))
		{
			system->AddEntity(entity);
		}
	}

	system->Init(this);
}

void Engine::RemoveSystem(std::shared_ptr<ISystem> system)
{
	std::vector<std::shared_ptr<ISystem>>::iterator systemIterator = std::find(m_Systems.begin(), m_Systems.end(), system);
	if (systemIterator == m_Systems.end())
	{
		return;
	}
	m_Systems.erase(systemIterator);
}

const std::shared_ptr<Window> Engine::GetWindow() const
{
	return m_Window;
}

void Engine::SetViewCenter(float X, float Y)
{
	m_Window->SetViewCenter(X, Y);
}

void Engine::GetViewCenter(float& X, float& Y)
{
	m_Window->GetViewPosition(X, Y);
}

#include "../Header Files/Engine.h"
#include "../Header Files/TextureManager.h"
#include "../Header Files/EventManager.h"
#include <algorithm>
#include <iostream>

Engine* Engine::s_pInstance = nullptr;

Engine* Engine::Instance()
{
	return s_pInstance;
}

Engine::Engine(int width, int height, std::string text, bool fullscreen)
{
	//setting up singleton
	s_pInstance = this;
	m_IsRunning = true;
	m_Window = std::make_shared<Window>(width, height, text, fullscreen);
	m_Time = std::make_unique<Time>();
	m_RenderSystem = std::make_shared<RenderSystem>();
	m_WorkerThread = std::thread(&Engine::UpdateWorkerSystems, this);
	m_RenderThread = std::thread(&Engine::UpdateRenderSystems, this);
	AddSystem(m_RenderSystem, eThreadImportance::direct);
	m_Window->SetWindowActive(false);
}

Engine::~Engine()
{
	if (s_pInstance == this)
	{
		s_pInstance = nullptr;
	}
	m_IsRunning = false;
	JoinThreads();
}

void Engine::Update()
{
	m_Time->Restart();

	const float elapsedTime = GetElapsedTimeAsSeconds();

	m_AccumulatedTime += elapsedTime;

	while (m_AccumulatedTime >= m_dt)
	{
		if (!m_Window->Update())
		{
			//Window has been closed
			m_IsRunning = false;
			return;
		}
		for (std::shared_ptr<ISystem> system : m_DirectSystems)
		{
			system->Update(this, m_dt);
		}
		//update all events
		EventManager::GetInstance().Update();
		//at the end of the update, decrement the accumulator by the fixed time
		m_AccumulatedTime -= m_dt;
	}
}

void Engine::UpdateWorkerSystems()
{
	std::cout << "Initializing worker thread" << std::endl;
	while (m_IsRunning)
	{
		while (m_AccumulatedTime >= m_dt)
		{
			//update all systems
			for (std::shared_ptr<ISystem> system : m_WorkerSystems)
			{
				system->Update(this, m_dt);
			}
			if (!m_IsRunning)
			{
				return;
			}
		}
	}
}

void Engine::UpdateRenderSystems()
{
	std::cout << "Initializing render thread" << std::endl;
	m_Window->SetWindowActive(true);
	while (m_IsRunning)
	{
		while (m_AccumulatedTime >= m_dt)
		{
			//update all systems
			for (std::shared_ptr<ISystem> system : m_RenderSystems)
			{
				system->Update(this, m_dt);
			}
			if (!m_IsRunning)
			{
				Draw();
				return;
			}
		}
		Draw();
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

	for (std::shared_ptr<ISystem> system : m_DirectSystems)
	{
		if (system->DoesEntityMatch(entity))
		{
			system->AddEntity(entity);
		}
	}

	for (std::shared_ptr<ISystem> system : m_WorkerSystems)
	{
		if (system->DoesEntityMatch(entity))
		{
			system->AddEntity(entity);
		}
	}

	for (std::shared_ptr<ISystem> system : m_RenderSystems)
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

	for (std::shared_ptr<ISystem> system : m_DirectSystems)
	{
		system->RemoveEntity(entity);
	}

	for (std::shared_ptr<ISystem> system : m_WorkerSystems)
	{
		system->RemoveEntity(entity);
	}

	for (std::shared_ptr<ISystem> system : m_RenderSystems)
	{
		system->RemoveEntity(entity);
	}
}

void Engine::AddSystem(std::shared_ptr<ISystem> system, eThreadImportance importance)
{
	switch (importance)
	{
	case eThreadImportance::direct:
		if (std::find(m_DirectSystems.begin(), m_DirectSystems.end(), system) != m_DirectSystems.end())
		{
			return;
		}
		m_DirectSystems.push_back(system);
		break;
	case eThreadImportance::worker:
		if (std::find(m_WorkerSystems.begin(), m_WorkerSystems.end(), system) != m_WorkerSystems.end())
		{
			return;
		}
		m_WorkerSystems.push_back(system);
		break;
	case eThreadImportance::render:
		if (std::find(m_RenderSystems.begin(), m_RenderSystems.end(), system) != m_RenderSystems.end())
		{
			return;
		}
		m_RenderSystems.push_back(system);
		break;
	default:
		break;
	}

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
	std::vector<std::shared_ptr<ISystem>>::iterator systemIterator = std::find(m_DirectSystems.begin(), m_DirectSystems.end(), system);
	if (systemIterator == m_DirectSystems.end())
	{
		std::vector<std::shared_ptr<ISystem>>::iterator systemIterator = std::find(m_WorkerSystems.begin(), m_WorkerSystems.end(), system);
		if (systemIterator == m_WorkerSystems.end())
		{
			std::vector<std::shared_ptr<ISystem>>::iterator systemIterator = std::find(m_RenderSystems.begin(), m_RenderSystems.end(), system);
			if (systemIterator == m_RenderSystems.end())
			{
				return;
			}
			m_RenderSystems.erase(systemIterator);
			return;
		}
		m_WorkerSystems.erase(systemIterator);
		return;
	}
	m_DirectSystems.erase(systemIterator);
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

#include "..\Header Files\MapSystem.h"
#include "..\Header Files\BlockSystem.h"
#include "..\Header Files\PhysicComponent.h"
#include "..\Header Files\BrickComponent.h"
#include "..\Header Files\IOComponent.h"
#include "..\Header Files\ScoreComponent.h"
#include <SFMLEngine/Header Files/TextComponent.h>
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <SFMLEngine/Header Files/Engine.h>
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <iostream>
#include <vector>
#include <cmath>

MapSystem::MapSystem()
{
	m_Listener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&MapSystem::OnEvent, this, std::placeholders::_1);
	m_Listener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_Listener);
}

MapSystem::~MapSystem()
{
	m_Listener->RemoveCallback(m_EventFunctor);
	EventManager::GetInstance().RemoveEventListener(m_Listener);
}

void MapSystem::Init(Engine* engine)
{
	LoadStartUI();
}

bool MapSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<BrickComponent>())
	{
		return true;
	}
	return false;
}

void MapSystem::Update(Engine* engine, float dt)
{
	eGameState state;
	Engine::Instance()->GetGameState(state);

	if (state == eGameState::start && engine->IsKeyPressed(Key::H))
	{
		std::shared_ptr<GameStartEvent> startEvent = std::make_shared<GameStartEvent>();
		EventManager::GetInstance().PushEvent(startEvent);
	}
	m_DeltaTime = dt;
}

void MapSystem::UpdateSingleEntityPosition(std::shared_ptr<Entity> entity, float dt)
{
	if (entity->HasComponent<PhysicComponent>())
	{
		int matrixPosX = 0;
		int matrixPosY = 0;
		entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
		entity->GetComponent<BrickComponent>()->SetBrickMatrixPosition(matrixPosX, matrixPosY + 1);
	}
}

void MapSystem::UpdateSingleEntityCollision(std::shared_ptr<Entity> entity, float dt)
{
	int mapSizeY = sizeof(m_MapMatrix) / sizeof(m_MapMatrix[0]);
	int mapSizeX = sizeof(m_MapMatrix[0]);
	bool collisionHasHappened = false;
	int matrixPosX = 0;
	int matrixPosY = 0;
	if (entity->HasComponent<PhysicComponent>())
	{
		entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> otherEntity = *entityItr;
			int matrixPosXOther = 0;
			int matrixPosYOther = 0;
			otherEntity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosXOther, matrixPosYOther);
			if (!otherEntity->HasComponent<PhysicComponent>() && matrixPosY + 1 == matrixPosYOther && matrixPosX == matrixPosXOther)
			{
				collisionHasHappened = true;
			}
			++entityItr;
		}
		if (matrixPosY >= mapSizeY - 1)
		{
			collisionHasHappened = true;
		}
	}
	
	if (collisionHasHappened)
	{
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;

		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
				m_MapMatrix[matrixPosY][matrixPosX] = true;
			}
			++entityItr;
		}

		int score = 0;
		bool didScore = false;
		for (int i = 0; i < mapSizeY; i++)
		{
			int counter = 0;
			for (int e = 0; e < mapSizeX; e++)
			{
				if (m_MapMatrix[i][e])
				{
					++counter;
				}
			}

			if (counter == mapSizeX)
			{
				for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
				{
					std::shared_ptr<Entity> entity = *entityItr;
					// Remove individual bricks
					entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
					if (matrixPosY == i)
					{
						m_MapMatrix[matrixPosY][matrixPosX] = false;
						Engine::Instance()->RemoveEntity(entity);
						didScore = true;
						score++;
					}
					++entityItr;
				}
				for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
				{
					std::shared_ptr<Entity> entity = *entityItr;
					// Move bricks above the deleted line down
					entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
					if (matrixPosY < i)
					{
						int currentMatrixPositionX = 0;
						int currentMatrixPositionY = 0;
						entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(currentMatrixPositionX, currentMatrixPositionY);
						entity->GetComponent<BrickComponent>()->SetBrickMatrixPosition(currentMatrixPositionX, currentMatrixPositionY + 1);
						float currentPositionX = 0;
						float currentPositionY = 0;
						entity->GetComponent<SpriteComponent>()->GetPosition(currentPositionX, currentPositionY);
						entity->GetComponent<SpriteComponent>()->SetPosition(currentPositionX, currentPositionY + 45.f);
					}
					++entityItr;
				}
			}
		}
		if (didScore) 
		{
			std::shared_ptr<ScoreEvent> scoreEvent = std::make_shared<ScoreEvent>();
			score = score / 10 * score / 10;
			scoreEvent->Score = score;
			EventManager::GetInstance().PushEvent(scoreEvent);
		}
		UpdateMap();
	}
	if (!m_CollisionHasHappened)
	{
		m_CollisionHasHappened = collisionHasHappened;
	}
}

void MapSystem::UpdateMap()
{
	for (auto & rows : m_MapMatrix)
	{
		for (auto & collumns : rows)
		{
			collumns = false;
		}
	}
	std::vector<std::shared_ptr<Entity>> copiedEntities = BlockSystem::Instance()->GetBrickEntities();;
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		std::shared_ptr<Entity> entity = *entityItr;
		if(entity->GetComponent<BrickComponent>())
		{
			int currentMatrixPositionX = 0;
			int currentMatrixPositionY = 0;
			entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(currentMatrixPositionX, currentMatrixPositionY);
			m_MapMatrix[currentMatrixPositionY][currentMatrixPositionX] = true;
			++entityItr;
		}
	}
}

void MapSystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<GameStartEvent> startEvent = std::dynamic_pointer_cast<GameStartEvent>(event);
	std::shared_ptr<LooseEvent> looseEvent = std::dynamic_pointer_cast<LooseEvent>(event);

	if (looseEvent != nullptr)
	{
		Engine::Instance()->SetGameState(eGameState::end);
	}

	if (startEvent != nullptr)
	{
		LoadGameUI();
		Engine::Instance()->SetGameState(eGameState::game);
	}

	std::shared_ptr<PhysicUpdateEvent> physicUpdateEvent = std::dynamic_pointer_cast<PhysicUpdateEvent>(event);

	if (physicUpdateEvent != nullptr)
	{
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			UpdateSingleEntityCollision(entity, m_DeltaTime);
			++entityItr;
		}

		if (m_CollisionHasHappened)
		{
			std::shared_ptr<CollisionEvent> collisionEvent = std::make_shared<CollisionEvent>();
			EventManager::GetInstance().PushEvent(collisionEvent);
			m_CollisionHasHappened = false;
		}
		else
		{
			for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
			{
				std::shared_ptr<Entity> entity = *entityItr;
				UpdateSingleEntityPosition(entity, m_DeltaTime);
				++entityItr;
			}
		}
	}
}

void MapSystem::LoadStartUI()
{
	std::shared_ptr<Entity> startGameUI = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> startGameText = startGameUI->AddComponent<TextComponent>();

	startGameText->SetFont("../bin/Jamma.ttf");
	startGameText->SetColor(255, 255, 255, 255); 
	int windowSizeX = 0;
	int windowSizeY = 0;
	Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
	startGameText->SetPosition(windowSizeX / 2, windowSizeY / 2);
	startGameText->SetSize(60);

	Engine::Instance()->AddEntity(startGameUI);
}

void MapSystem::LoadGameUI()
{
	std::shared_ptr<Entity> iOEntity = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> iOText = iOEntity->AddComponent<TextComponent>();
	std::shared_ptr<IOComponent> iOComp = iOEntity->AddComponent<IOComponent>();

	iOText->SetFont("../bin/Jamma.ttf");
	iOText->SetColor(255, 255, 255, 255);
	iOText->SetPosition(10.0f, 10.0f);
	iOText->SetSize(24);

	Engine::Instance()->AddEntity(iOEntity);

	std::shared_ptr<Entity> scoreEntity = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> scoreText = scoreEntity->AddComponent<TextComponent>();
	std::shared_ptr<ScoreComponent> scoreComp = scoreEntity->AddComponent<ScoreComponent>();

	scoreText->SetFont("../bin/Jamma.ttf");
	scoreText->SetColor(255, 255, 255, 255);
	scoreText->SetPosition(10.0f, 10.0f);
	scoreText->SetSize(24);

	Engine::Instance()->AddEntity(scoreEntity);
}


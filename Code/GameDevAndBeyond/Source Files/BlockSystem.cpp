#include "../Header Files/BlockSystem.h"
#include "../Header Files/PhysicComponent.h"
#include "../Header Files/BrickComponent.h"
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <SFMLEngine/Header Files/Engine.h>
#include <iostream>
#include <ctime>

BlockSystem* BlockSystem::s_pInstance = nullptr;

BlockSystem* BlockSystem::Instance()
{
	return s_pInstance;
}

BlockSystem::BlockSystem()
{
	//setting up singleton
	s_pInstance = this;
	//setting up events
	m_EventListener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&BlockSystem::OnEvent, this, std::placeholders::_1);
	m_EventListener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_EventListener);
	//setting up different shapes for the blocks
	BlockShape longBlock = { 1,1,1,1,
								0,0,0,0 };
	BlockShape tBlock = { 0,1,1,1,
								0,0,1,0 };
	BlockShape leftZBlock = { 0,1,1,0,
								0,0,1,1 };
	BlockShape rightZBlock = { 0,1,1,0,
								1,1,0,0 };
	BlockShape leftLBlock = { 0,1,1,1,
								0,1,0,0 };
	BlockShape rightLBlock = { 1,1,1,0,
								0,0,1,0 };
	BlockShape twoByTwoBlock = { 0,1,1,0,
								0,1,1,0 };
	m_BlockShapes.insert(std::pair<int, BlockShape>(0, longBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(1, tBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(2, leftZBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(3, rightZBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(4, leftLBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(5, rightLBlock));
	m_BlockShapes.insert(std::pair<int, BlockShape>(6, twoByTwoBlock));
}

BlockSystem::~BlockSystem()
{
	if (s_pInstance == this)
	{
		s_pInstance = nullptr;
	}
	//End events
	EventManager::GetInstance().RemoveEventListener(m_EventListener);
	m_EventListener->RemoveCallback(m_EventFunctor);
}

void BlockSystem::Init(Engine* engine)
{
}

bool BlockSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<PhysicComponent>())
	{
		int matrixPosX = 0;
		int matrixPosY = 0;
		entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
		return true;
	}
	return false;
}

void BlockSystem::Update(Engine* engine, float dt)
{
	if (!Engine::Instance()->IsRunning())
	{
		return;
	}
	eGameState state;
	Engine::Instance()->GetGameState(state);
	if (state != eGameState::game)
	{
		return;
	}
	UpdateEntities(engine, dt);
}

inline bool BlockSystem::UpdateEntities(Engine* engine, float dt)
{
	if (m_ReadyForDropKeyPress)
	{
		Drop(engine);
	}
	if (m_ReadyForLeftKeyPress)
	{
		MoveLeft(engine);
	}
	if (m_ReadyForRightKeyPress)
	{
		MoveRight(engine);
	}
	if (m_ReadyForRotateKeyPress)
	{
		Rotation(engine);
	}
	const float coolDownTime = 0.15f;

	if (!m_ReadyForLeftKeyPress || !m_ReadyForDropKeyPress || !m_ReadyForRightKeyPress || !m_ReadyForRotateKeyPress)
	{
		m_TickTime += dt;
	}
	if (m_TickTime >= coolDownTime)
	{
		m_ReadyForLeftKeyPress = true;
		m_ReadyForDropKeyPress = true;
		m_ReadyForRightKeyPress = true;
		m_ReadyForRotateKeyPress = true;
		m_TickTime = 0;
	}
	return true;
}

inline void BlockSystem::MoveLeft(Engine* engine)
{
	if (engine->IsKeyPressed(Key::A) || engine->IsKeyPressed(Key::Left))
	{
		m_ReadyForLeftKeyPress = false;
		bool canMoveLeft = true;
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				int matrixPosX = 0;
				int matrixPosY = 0;
				entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
				for (std::vector<std::shared_ptr<Entity>>::iterator otherEntityItr = copiedEntities.begin(); otherEntityItr != copiedEntities.end();)
				{
					std::shared_ptr<Entity> otherEntity = *otherEntityItr;
					if (!otherEntity->HasComponent<PhysicComponent>())
					{
						int otherMatrixPosX = 0;
						int otherMatrixPosY = 0;
						otherEntity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(otherMatrixPosX, otherMatrixPosY);
						if (otherMatrixPosY == matrixPosY && otherMatrixPosX == matrixPosX - 1)
						{
							canMoveLeft = false;
						}
					}
					if (matrixPosX <= 0)
					{
						canMoveLeft = false;
					}
					++otherEntityItr;
				}
			}
			++entityItr;
		}
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			std::shared_ptr<SpriteComponent> spriteComponent = entity->GetComponent<SpriteComponent>();
			int matrixPosX = 0;
			int matrixPosY = 0;
			entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
			if (canMoveLeft && entity->HasComponent<PhysicComponent>())
			{
				float posX;
				float posY;
				spriteComponent->GetPosition(posX, posY);
				spriteComponent->SetPosition(posX - 45.f, posY);
				entity->GetComponent<BrickComponent>()->SetBrickMatrixPosition(matrixPosX - 1, matrixPosY);
			}
			++entityItr;
		}
	}
}

inline void BlockSystem::MoveRight(Engine* engine)
{
	if (engine->IsKeyPressed(Key::D) || engine->IsKeyPressed(Key::Right))
	{
		m_ReadyForRightKeyPress = false;
		bool canMoveRight = true;
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				int matrixPosX = 0;
				int matrixPosY = 0;
				entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
				for (std::vector<std::shared_ptr<Entity>>::iterator otherEntityItr = copiedEntities.begin(); otherEntityItr != copiedEntities.end();)
				{
					std::shared_ptr<Entity> otherEntity = *otherEntityItr;
					if (!otherEntity->HasComponent<PhysicComponent>())
					{
						int otherMatrixPosX = 0;
						int otherMatrixPosY = 0;
						otherEntity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(otherMatrixPosX, otherMatrixPosY);
						if (otherMatrixPosY == matrixPosY && otherMatrixPosX == matrixPosX + 1)
						{
							canMoveRight = false;
						}
					}
					if (matrixPosX >= 9)
					{
						canMoveRight = false;
					}
					++otherEntityItr;
				}
			}
			++entityItr;
		}
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			int matrixPosX = 0;
			int matrixPosY = 0;
			entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
			if (canMoveRight && entity->HasComponent<PhysicComponent>())
			{
				float posX;
				float posY;
				std::shared_ptr<SpriteComponent> spriteComponent = entity->GetComponent<SpriteComponent>();
				spriteComponent->GetPosition(posX, posY);
				spriteComponent->SetPosition(posX + 45.f, posY);
				entity->GetComponent<BrickComponent>()->SetBrickMatrixPosition(matrixPosX + 1, matrixPosY);
			}
			++entityItr;
		}
	}
}

inline void BlockSystem::Drop(Engine* engine)
{
	if (engine->IsKeyPressed(Key::S) || engine->IsKeyPressed(Key::Down))
	{
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				float waitTime = 0.05f;
				entity->GetComponent<PhysicComponent>()->SetUpdateWaitTime(waitTime);
			}
			++entityItr;
		}
		m_ReadyForDropKeyPress = false;
	}
}

inline void BlockSystem::Rotation(Engine* engine)
{
	if (engine->IsKeyPressed(Key::Space) || engine->IsKeyPressed(Key::Up) || engine->IsKeyPressed(Key::W))
	{
		int mostLeftMatrixPosition = 100;
		int mostTopMatrixPosition = 100;
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		std::vector<std::shared_ptr<Entity>> movedEntities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			std::shared_ptr<Entity> entity = *entityItr;
			if (entity->HasComponent<PhysicComponent>())
			{
				movedEntities.push_back(entity);
				int matrixPosX = 0;
				int matrixPosY = 0;
				entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
				if (matrixPosX < mostLeftMatrixPosition)
				{
					mostLeftMatrixPosition = matrixPosX;
				}
				if (matrixPosY < mostTopMatrixPosition)
				{
					mostTopMatrixPosition = matrixPosY;
				}
			}
			++entityItr;
		}

		int movedEntitiesCounter = 0;
		++m_CurrentRotation;
		bool rotationArray[4][4]{ 0 };
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				rotationArray[i][j] = m_BlockShapes[m_ShapeKey].BlockShape[i][j];
			}
		}
		for (int rotationCount = 0; rotationCount < m_CurrentRotation; ++rotationCount)
		{
			for (int i = 0; i < 4 / 2; i++) {
				for (int j = i; j < 4 - i - 1; j++) {
					int temp = rotationArray[i][j];
					rotationArray[i][j] = rotationArray[4 - 1 - j][i];
					rotationArray[4 - 1 - j][i] = rotationArray[4 - 1 - i][4 - 1 - j];
					rotationArray[4 - 1 - i][4 - 1 - j] = rotationArray[j][4 - 1 - i];
					rotationArray[j][4 - 1 - i] = temp;
				}
			}
		}
		int mostLeftElement = 100;
		int mostTopElement = 100;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (rotationArray[i][j] && j < mostLeftElement)
				{
					mostLeftElement = j;
				}
				if (rotationArray[i][j] && i < mostTopElement)
				{
					mostTopElement = i;
				}
			}
		}
		int blockLength = 0;
		for (int j = 0; j < 4; j++)
		{
			bool rowContainsBrick = false;
			for (int i = 0; i < 4; i++)
			{
				if (rotationArray[i][j])
				{
					rowContainsBrick = true;
					rotationArray[i][j] = false;
					rotationArray[i - mostTopElement][j - mostLeftElement] = true;
				}
			}
			if (rowContainsBrick)
			{
				blockLength++;
			}
			if (blockLength + mostLeftMatrixPosition > 10)
			{
				mostLeftMatrixPosition -= ((mostLeftMatrixPosition + blockLength) % 10) - 1;
			}
		}

		std::vector<std::shared_ptr<Entity>> unmovedEntities;
		GetUnmovedEntities(copiedEntities, unmovedEntities);

		bool retflag;
		CheckIfRotationPossible(rotationArray, movedEntities, movedEntitiesCounter, unmovedEntities, retflag);
		if (retflag)
		{
			--m_CurrentRotation;
			return;
		}
		movedEntitiesCounter = 0;
		MoveBrickEntities(rotationArray, movedEntities, movedEntitiesCounter, mostLeftMatrixPosition, mostTopMatrixPosition);

		if (m_CurrentRotation >= m_MaximumRotations)
		{
			m_CurrentRotation = 0;
		}
		m_ReadyForRotateKeyPress = false;
	}
}

inline void BlockSystem::GetUnmovedEntities(std::vector<std::shared_ptr<Entity>>& copiedEntities, std::vector<std::shared_ptr<Entity>>& unmovedEntities)
{
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		std::shared_ptr<Entity> entity = *entityItr;
		if (!entity->HasComponent<PhysicComponent>())
		{
			unmovedEntities.push_back(entity);
		}
		++entityItr;
	}
}

inline void BlockSystem::MoveBrickEntities(bool rotationArray[4][4],
	std::vector<std::shared_ptr<Entity>>& movedEntities,
	int& movedEntitiesCounter,
	int mostLeftMatrixPosition,
	int mostTopMatrixPosition)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int e = 0; e < 4; ++e)
		{
			if (rotationArray[i][e])
			{
				std::shared_ptr<BrickComponent> brickComponent = movedEntities[movedEntitiesCounter]->GetComponent<BrickComponent>();
				brickComponent->SetBrickMatrixPosition(mostLeftMatrixPosition + e, mostTopMatrixPosition + i);
				SetSpritePosOnWindow(movedEntities, movedEntitiesCounter, mostLeftMatrixPosition + e, mostTopMatrixPosition + i);
				++movedEntitiesCounter;
			}
		}
	}
}

inline void BlockSystem::CheckIfRotationPossible(bool rotationArray[4][4], std::vector<std::shared_ptr<Entity>>& movedEntities, int& movedEntitiesCounter, std::vector<std::shared_ptr<Entity>>& unmovedEntities, bool& retflag)
{
	retflag = true;
	for (int i = 0; i < 4; ++i)
	{
		for (int e = 0; e < 4; ++e)
		{
			if (rotationArray[i][e])
			{
				std::shared_ptr<BrickComponent> brickComponent = movedEntities[movedEntitiesCounter]->GetComponent<BrickComponent>();
				int movedMatrixPosX = 0;
				int movedMatrixPosY = 0;
				brickComponent->GetBrickMatrixPosition(movedMatrixPosX, movedMatrixPosY);

				if (movedMatrixPosX < 0 || movedMatrixPosX >= 9)
				{
					return;
				}
				movedMatrixPosX += e;
				movedMatrixPosY += i;
				for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = unmovedEntities.begin(); entityItr != unmovedEntities.end();)
				{
					std::shared_ptr<Entity> entity = *entityItr;
					std::shared_ptr<BrickComponent> unmovedBrickComponent = entity->GetComponent<BrickComponent>();
					int unmovedMatrixPosX = 0;
					int unmovedMatrixPosY = 0;
					unmovedBrickComponent->GetBrickMatrixPosition(unmovedMatrixPosX, unmovedMatrixPosY);
					if (unmovedMatrixPosX == movedMatrixPosX && unmovedMatrixPosY == movedMatrixPosY)
					{
						return;
					}
					++entityItr;
				}
				++movedEntitiesCounter;
			}
		}
	}
	retflag = false;
}

inline void BlockSystem::SetSpritePosOnWindow(std::vector<std::shared_ptr<Entity>>& movedEntities,
	int movedEntitiesCounter,
	int brickMatrixPosX,
	int brickMatrixPosY)
{
	std::shared_ptr<SpriteComponent> spriteComponent = movedEntities[movedEntitiesCounter]->GetComponent<SpriteComponent>();
	float spritePosX = brickMatrixPosX * 45;
	float spritePosY = brickMatrixPosY * 45;
	int windowSizeX = 0;
	int windowSizeY = 0;
	Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
	spriteComponent->SetPosition((brickMatrixPosX) * 45 + windowSizeX / 3, brickMatrixPosY * 45 + windowSizeY / 20);
}

void BlockSystem::SpawnBlock()
{
	eGameState state;
	Engine::Instance()->GetGameState(state);
	if (state == eGameState::end)
	{
		return;
	}
	ResetPosition();
	m_CurrentRotation = 0;
	srand(time(nullptr));
	m_ShapeKey = (rand() % 7);
	for (int i = 0; i < 2; ++i)
	{
		for (int e = 0; e < 4; ++e)
		{
			if (m_BlockShapes[m_ShapeKey].BlockShape[i][e])
			{
				std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
				for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
				{
					std::shared_ptr<Entity> entity = *entityItr;
					int matrixPosX = 0;
					int matrixPosY = 0;
					entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
					if (matrixPosX == e + 4 && matrixPosY == i)
					{
						std::shared_ptr<LooseEvent> looseEvent = std::make_shared<LooseEvent>();
						EventManager::GetInstance().PushEvent(looseEvent);
						return;
					}
					++entityItr;
				}
				std::shared_ptr<Entity> brickEntity = std::make_shared<Entity>();
				std::shared_ptr<BrickComponent> brickComponent = brickEntity->AddComponent<BrickComponent>();
				brickComponent->SetBrickMatrixPosition(e + 4, i);
				std::shared_ptr<SpriteComponent> brickSprite = brickEntity->AddComponent<SpriteComponent>();
				std::shared_ptr<PhysicComponent> physicComponent = brickEntity->AddComponent<PhysicComponent>();
				brickSprite->CreateSprite("../bin/Brick.png");
				brickSprite->SetColor(m_ShapeKey);
				int windowSizeX = 0;
				int windowSizeY = 0;
				Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
				brickSprite->SetPosition((e + 4) * 45 + windowSizeX / 3, i * 45 + static_cast<int>(windowSizeY / 20));
				Engine::Instance()->AddEntity(brickEntity);
			}
		}
	}
}

void BlockSystem::ResetPosition()
{
	int windowSizeX = 0;
	int windowSizeY = 0;
	Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		std::shared_ptr<Entity> entity = *entityItr;
		int matrixPosX = 0;
		int matrixPosY = 0;
		entity->GetComponent<BrickComponent>()->GetBrickMatrixPosition(matrixPosX, matrixPosY);
		entity->GetComponent<SpriteComponent>()->SetPosition(matrixPosX * 45 + windowSizeX / 3, matrixPosY * 45 + static_cast<int>(windowSizeY / 20));
		++entityItr;
	}
}

void BlockSystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<GameStartEvent> startEvent = std::dynamic_pointer_cast<GameStartEvent>(event);
	std::shared_ptr<SpawnEvent> spawnEvent = std::dynamic_pointer_cast<SpawnEvent>(event);
	std::shared_ptr<ScoreEvent> scoreEvent = std::dynamic_pointer_cast<ScoreEvent>(event);
	std::shared_ptr<LooseEvent> looseEvent = std::dynamic_pointer_cast<LooseEvent>(event);
	if (startEvent != nullptr)
	{
		SpawnBlock();
	}
	if (looseEvent != nullptr)
	{
		//TODO: Make end, stop physicupdates
	}
	if (scoreEvent != nullptr)
	{
	}
	if (spawnEvent != nullptr)
	{
		SpawnBlock();
	}
}
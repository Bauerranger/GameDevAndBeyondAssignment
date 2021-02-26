#include "../Header Files/UISystem.h"
#include "../Header Files/ScoreComponent.h"
#include "../Header Files/IOComponent.h"
#include <SFMLEngine/Header Files/TextComponent.h>
#include <SFMLEngine/Header Files/InputHelper.h>
#include <iostream>
#include <string>

UISystem::UISystem()
{
	m_Listener = std::make_shared<EventHandler>();
	m_EventFunctor = std::bind(&UISystem::OnEvent, this, std::placeholders::_1);
	m_Listener->AddCallback(m_EventFunctor);
	EventManager::GetInstance().AddEventListener(m_Listener);
}

UISystem::~UISystem()
{
	EventManager::GetInstance().RemoveEventListener(m_Listener);
	m_Listener->RemoveCallback(m_EventFunctor);
}

bool UISystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<ScoreComponent>())
	{
		std::shared_ptr<ScoreComponent> scoreComp = entity->GetComponent<ScoreComponent>();
		scoreComp->GetScore(m_Score);
	}
	if (entity->HasComponent<TextComponent>())
	{
		return true;
	}
	return false;
}
// TODO: only update when needed
void UISystem::Update(Engine* engine, float dt)
{
	if (m_Entities.size() == 0)
	{
		return;
	}
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		if (!Engine::Instance && !Engine::Instance()->IsRunning())
		{
			return;
		}
		eGameState state;
		Engine::Instance()->GetGameState(state);
		std::shared_ptr<Entity> entity = *entityItr;
		std::shared_ptr<TextComponent> textComp = entity->GetComponent<TextComponent>();
		std::shared_ptr<ScoreComponent> scoreComp = entity->GetComponent<ScoreComponent>();
		std::shared_ptr<IOComponent> iOComp = entity->GetComponent<IOComponent>();

		if (scoreComp != nullptr)
		{
			scoreComp->SetScore(m_Score);
			textComp->SetText("Score: " + std::to_string(m_Score));
		}

		if (iOComp != nullptr)
		{
			if (state == eGameState::end)
			{
				textComp->SetSize(48);
				int highscore = 0;
				std::string highscoreChampion;
				iOComp->GetScore(highscoreChampion, highscore);
				char input = A;
				InputHelper::GetInput(input);
				if (isalpha(input))
				{
					m_Name = m_Name + input;
				}
				if (highscore >= m_Score)
				{
					textComp->SetText("Current highscore: " + std::to_string(highscore) + " by: " + highscoreChampion);
					//Delete me
					/*
					textComp->SetText("New highscore: " + std::to_string(m_Score) + " Enter your name: " + m_Name);
					if (engine->IsKeyPressed(Key::Enter))
					{
						iOComp->SetScore(m_Name, m_Score);
						iOComp->SaveScoreToDrive();
					}*/
				}
				if (highscore < m_Score)
				{
					textComp->SetText("New highscore: " + std::to_string(m_Score) + " Enter your name: " + m_Name);
					if (engine->IsKeyPressed(Key::Enter))
					{
						iOComp->SetScore(m_Name, m_Score);
						iOComp->SaveScoreToDrive();
					}
				}
			}
		}
		++entityItr;
	}
}

void UISystem::Init(Engine* engine)
{
	LoadStartUI();
}

void UISystem::LoadStartUI()
{
	if (!Engine::Instance)
	{
		return;
	}

	// Start Text
	{
		int windowSizeX = 0;
		int windowSizeY = 0;
		Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
		// For some reason the center does not center
		int posX = windowSizeX / 2 - 50;
		int posY = windowSizeY / 2;

		std::shared_ptr<Entity> startGameUI = std::make_shared<Entity>();
		CreateTextEntity(startGameUI, 255, 255, 255, 255,
			"PRESS SPACE TO START GAME",
			true, posX, posY, 32, eGameState::start);

		Engine::Instance()->AddEntity(startGameUI);
	}

	// Attribution text
	{
		int windowSizeX = 0;
		int windowSizeY = 0;
		Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
		int posX = windowSizeX;
		int posY = windowSizeY - 40;

		std::shared_ptr<Entity> attributionUI = std::make_shared<Entity>();

		CreateTextEntity(attributionUI, 255, 255, 255, 255,
			"Tetris Song: Bogozi, CC BY-SA 3.0 <https://creativecommons.org/licenses/by-sa/3.0>, via Wikimedia Commons",
			true, posX, posY, 12, eGameState::start);

		Engine::Instance()->AddEntity(attributionUI);
	}
	// Load Score Text
	{
		std::shared_ptr<Entity> scoreEntity = std::make_shared<Entity>();
		CreateTextEntity(scoreEntity, 255, 255, 255, 255,
			"score", false, 10, 10, 24,
			eGameState::game);

		std::shared_ptr<ScoreComponent> scoreComp = scoreEntity->AddComponent<ScoreComponent>();

		Engine::Instance()->AddEntity(scoreEntity);
	}
	// Load High Score Text
	{
		int windowSizeX = 0;
		int windowSizeY = 0;
		Engine::Instance()->GetWindow()->GetWindowSize(windowSizeX, windowSizeY);
		int posX = windowSizeX / 5;
		int posY = windowSizeY / 2;
		std::shared_ptr<Entity> iOEntity = std::make_shared<Entity>();
		CreateTextEntity(iOEntity, 255, 255, 255, 255,
			"IO", true, posX, posY, 24,
			eGameState::end);

		std::shared_ptr<IOComponent> iOComp = iOEntity->AddComponent<IOComponent>();

		Engine::Instance()->AddEntity(iOEntity);
	}
}

void UISystem::CreateTextEntity(std::shared_ptr<Entity> entity, const int red, const int green, const int blue, const int alpha, const std::string text, const bool centerText,
	const int posX, const int posY, const int textSize, const eGameState visibleGameState)
{
	std::shared_ptr<TextComponent> textComponent = entity->AddComponent<TextComponent>();

	textComponent->SetFont("../bin/Jamma.ttf");
	textComponent->SetColor(red, green, blue, alpha);
	textComponent->SetText(text);
	if (centerText)
		textComponent->CenterText();
	// For some reason the center does not center
	textComponent->SetPosition(float(posX), float(posY));
	textComponent->SetSize(textSize);
	textComponent->SetVisibleState(visibleGameState);
	textComponent->SetVisibilityOnStateChange();
}

void UISystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<GameStateChangeEvent> changeEvent = std::dynamic_pointer_cast<GameStateChangeEvent>(event);
	std::shared_ptr<ScoreEvent> scoreEvent = std::dynamic_pointer_cast<ScoreEvent>(event);

	if (scoreEvent != nullptr)
	{
		m_Score += scoreEvent->Score;
		// Calculates when the next level is reached
		if (m_Level < 11 && m_Score >= pow(m_Level + 1, 2) + m_Level * 16)
		{
			std::shared_ptr<LevelUpEvent> levelUpEvent = std::make_shared<LevelUpEvent>();
			EventManager::GetInstance().PushEvent(levelUpEvent);
			m_Level++;
		}
	}

	if (changeEvent != nullptr)
	{
		std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
		for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
		{
			if (!Engine::Instance && !Engine::Instance()->IsRunning())
			{
				return;
			}
			std::shared_ptr<Entity> entity = *entityItr;
			std::shared_ptr<TextComponent> textComp = entity->GetComponent<TextComponent>();
			textComp->SetVisibilityOnStateChange();
			entityItr++;
		}
	}
}
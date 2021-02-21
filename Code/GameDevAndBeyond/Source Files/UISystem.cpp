#include "../Header Files/UISystem.h"
#include "../Header Files/ScoreComponent.h"
#include "../Header Files/IOComponent.h"
#include <SFMLEngine/Header Files/Engine.h>
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

void UISystem::Update(Engine* engine, float dt)
{
	if (m_Entities.size() == 0)
	{
		return;
	}
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (std::vector<std::shared_ptr<Entity>>::iterator entityItr = copiedEntities.begin(); entityItr != copiedEntities.end();)
	{
		std::shared_ptr<Entity> entity = *entityItr;
		std::shared_ptr<TextComponent> textComp = entity->GetComponent<TextComponent>();
		std::shared_ptr<ScoreComponent> scoreComp = entity->GetComponent<ScoreComponent>();
		std::shared_ptr<IOComponent> iOComp = entity->GetComponent<IOComponent>();
		if (scoreComp != nullptr)
		{
			scoreComp->SetScore(m_Score);
			if (m_Dead)
			{
				textComp->SetPosition(1600 / 5, 900 / 3);
				textComp->SetSize(48);
				textComp->SetText("You are dead! Your score is\n" + std::to_string(m_Score));
			}
			else if (m_Won)
			{
				textComp->SetPosition(1600 / 5, 900 / 3);
				textComp->SetSize(48);
				textComp->SetText("You won! Your score is\n" + std::to_string(m_Score));
			}
			else
			{
				textComp->SetText("Score: " + std::to_string(m_Score));
			}
		}
		if (iOComp != nullptr)
		{
			if (m_Dead || m_Won)
			{
				textComp->SetPosition(1600 / 5, 900 / 2);
				textComp->SetSize(48);
				int highscore = 0;
				std::string highscoreChampion = "";
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

					textComp->SetText("New highscore: " + std::to_string(m_Score) + " Enter your name: " + m_Name);
					if (engine->IsKeyPressed(Key::Enter))
					{
						iOComp->SetScore(m_Name, m_Score);
						iOComp->SaveScoreToDrive();
					}
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

void UISystem::OnEvent(std::shared_ptr<IEvent> event)
{
	std::shared_ptr<ScoreEvent> scoreEvent = std::dynamic_pointer_cast<ScoreEvent>(event);
	if (scoreEvent != nullptr)
	{
		m_Score += scoreEvent->Score;
	}
}

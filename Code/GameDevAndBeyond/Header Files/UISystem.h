#pragma once

#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include <SFML/Graphics.hpp>
#include <SFMLEngine/Header Files/Engine.h>
#include "Events.h"

class UISystem : public ISystem
{
public:
	UISystem();
	~UISystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	virtual void Update(Engine* engine, float dt) override;

	void Init(Engine* engine) override;

	void OnEvent(std::shared_ptr<IEvent> event);
private:
	void LoadStartUI();

	void CreateTextEntity(std::shared_ptr<Entity> entity, const int red, const int green, const int blue, const int alpha, const std::string text, const bool centerText,
		const int posX, const int posY, const int textSize, const eGameState visibleGameState);

	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_EventFunctor;
	int m_Score = 0;
	int m_Health = 0;

	std::string m_Name;
	int m_Level = 1;
};
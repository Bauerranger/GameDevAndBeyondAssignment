#pragma once

#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
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

	void OnScoreChange(std::shared_ptr<IEvent> event);
private:
	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_ScoreEventFunctor;
	int m_Score = 0;
	int m_Health = 0;
	bool m_Dead = false;
	bool m_Won = false;
	std::string m_Name = "";
};
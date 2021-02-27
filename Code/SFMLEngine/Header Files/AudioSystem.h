#pragma once
#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include "GameDevAndBeyond/Header Files/AudioComponent.h"
#include "GameDevAndBeyond/Header Files/Events.h"

class AudioSystem : public ISystem
{
public:
	AudioSystem();
	~AudioSystem();
public:
	//ISystem
	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	void Update(Engine* engine, float dt) override;
	void Init(Engine* engine) override;
	void AddEntity(std::shared_ptr<Entity> entity) override;
private:
	bool inline UpdateSingleEntity(Engine* engine, std::shared_ptr<Entity> entity, float dt);

	void PlayMusic();
	void PlayScore();
	void PlayCollision();
	void PlayEnd();

	void OnEvent(std::shared_ptr<IEvent> event);
	std::shared_ptr<EventHandler> m_Listener;
	EventFunctor m_EventFunctor;
	std::shared_ptr<Entity> m_Music;
};
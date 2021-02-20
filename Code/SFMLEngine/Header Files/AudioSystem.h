#pragma once
#include "SFMLEngine/Header Files/ISystem.h"
#include "GameDevAndBeyond/Header Files/AudioComponent.h"

class AudioSystem : public ISystem
{
public:
	AudioSystem();
	~AudioSystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	virtual void Update(Engine* engine, float dt) override;
	virtual void Init(Engine* engine) override;
	virtual void AddEntity(std::shared_ptr<Entity> entity) override;
private:
	bool inline UpdateSingleEntity(Engine* engine, std::shared_ptr<Entity> entity, float dt);

};
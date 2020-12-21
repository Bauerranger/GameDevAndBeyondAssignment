#pragma once
#include "SFMLEngine/Header Files/ISystem.h"
#include "AudioComponent.h"

class AudioSystem : public ISystem
{
public:
	AudioSystem();
	~AudioSystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	//virtual void Update(Engine* engine, float dt) override;
private:
	//bool inline UpdateSingleEntity(Engine* engine, std::shared_ptr<Entity> entity, float dt);
public:

	void StartAudio(std::string audioFile, std::shared_ptr<AudioComponent> pAudioProperties, bool audioLoops);
};
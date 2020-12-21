#include "../Header Files/AudioSystem.h"

AudioSystem::AudioSystem()
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	return false;
}

void AudioSystem::StartAudio(std::string audioFile, std::shared_ptr<AudioComponent> pAudioProperties, bool audioLoops)
{
	//m_musicEntityId = entitySystem.AddEntity();
	//AudioProperties* pAudioProperties = entitySystem.AddComponent<AudioProperties>(m_musicEntityId);
	pAudioProperties->m_pSoundBuffer = std::make_shared<sf::SoundBuffer>();
	pAudioProperties->m_pSound = std::make_shared<sf::Sound>();
	pAudioProperties->m_pSoundBuffer->loadFromFile(audioFile);
	pAudioProperties->m_pSound->setBuffer(*pAudioProperties->m_pSoundBuffer);
	pAudioProperties->m_pSound->setLoop(audioLoops);
}
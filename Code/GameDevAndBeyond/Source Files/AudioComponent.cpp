#include "../Header Files/AudioComponent.h"
#include <iostream>

AudioComponent::AudioComponent(const std::string fileName, const bool isLooping)
	: m_FileName(fileName), m_IsLooping(isLooping)
{ 
	m_pSoundBuffer = std::make_shared<sf::SoundBuffer>();
	m_pSound = std::make_shared<sf::Sound>();
	if (!m_pSoundBuffer->loadFromFile(m_FileName))
		std::cout << "ERROR: Could not load sound file from " << m_FileName << std::endl;
	m_pSound->setBuffer(*m_pSoundBuffer);
	m_pSound->setLoop(m_IsLooping);
}

AudioComponent::~AudioComponent()
{	
}

AudioComponent& AudioComponent::operator=(const AudioComponent& other)
{
	m_pSoundBuffer = other.m_pSoundBuffer;
	m_pSound = other.m_pSound;

	if (m_pSound && m_pSoundBuffer)
	{
		m_pSound->setBuffer(*m_pSoundBuffer);
	}

	return *this;
}

//Copy Constructor
AudioComponent::AudioComponent(const AudioComponent& other)
	: m_FileName(other.m_FileName), m_IsLooping(other.m_IsLooping)
{
	m_pSoundBuffer = other.m_pSoundBuffer;
	m_pSound = other.m_pSound;

	if (m_pSound && m_pSoundBuffer)
	{
		m_pSound->setBuffer(*m_pSoundBuffer);
	}
}

//Move constructor
AudioComponent::AudioComponent(AudioComponent&& other) noexcept
	: m_FileName(other.m_FileName), m_IsLooping(other.m_IsLooping)
{
	m_pSoundBuffer = other.m_pSoundBuffer;
	m_pSound = other.m_pSound;

	if (m_pSound && m_pSoundBuffer)
	{
		m_pSound->setBuffer(*m_pSoundBuffer);
	}

	other.m_pSound.reset();
	other.m_pSoundBuffer.reset();
}

bool AudioComponent::AudioIsFinished()
{
	return false;
}

void AudioComponent::PlayAudio()
{
	m_pSound->play();
}

void AudioComponent::PauseAudio()
{
	m_pSound->pause();
}

void AudioComponent::StopAudio()
{
	m_pSound->stop();
}

const float AudioComponent::GetAudioLength() 
{ 
	return m_pSoundBuffer->getDuration().asSeconds(); 
}

void AudioComponent::CalculatePlayedTime(float dt) 
{ 
	m_PlayedTime += dt;
}
#include "../Header Files/AudioComponent.h"

AudioComponent::AudioComponent()
{ 
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

const float AudioComponent::GetAudioLength() 
{ 
	return m_pSoundBuffer->getDuration().asSeconds(); 
}

void AudioComponent::CalculatePlayedTime(float dt) 
{ 
	m_PlayedTime += dt;
}
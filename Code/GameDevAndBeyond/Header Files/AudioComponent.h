#pragma once
#include "SFML/Audio.hpp"
#include "SFMLEngine/Header Files/IComponent.h"


class AudioComponent : public IComponent
{
public:
	//Constructor
	AudioComponent();
	~AudioComponent();

	//Copy assignment constructor
	AudioComponent& operator=(const AudioComponent& other);

	//Copy Constructor
	AudioComponent(const AudioComponent& other);

	//Move constructor
	AudioComponent(AudioComponent&& other) noexcept;

	std::shared_ptr<sf::SoundBuffer> m_pSoundBuffer;
	std::shared_ptr<sf::Sound> m_pSound;

private:
	const float GetAudioLength();
	float m_PlayedTime = 0;
	void CalculatePlayedTime(float dt);
};
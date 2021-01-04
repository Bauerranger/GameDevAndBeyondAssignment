#pragma once
#include "SFML/Audio.hpp"
#include "SFMLEngine/Header Files/IComponent.h"


class AudioComponent : public IComponent
{
public:
	//Constructor
	AudioComponent(const std::string fileName, const bool isLooping);
	~AudioComponent();

	//Copy assignment constructor
	AudioComponent& operator=(const AudioComponent& other);

	//Copy Constructor
	AudioComponent(const AudioComponent& other);

	//Move constructor
	AudioComponent(AudioComponent&& other) noexcept;

	std::shared_ptr<sf::SoundBuffer> m_pSoundBuffer;
	std::shared_ptr<sf::Sound> m_pSound;

	bool AudioIsFinished();
	void PlayAudio();
	void PauseAudio();
	void StopAudio();


private:
	const float GetAudioLength();
	void CalculatePlayedTime(float dt);

	float m_PlayedTime = 0;
	const std::string m_FileName;
	const bool m_IsLooping;
};
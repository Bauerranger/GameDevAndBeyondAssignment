#pragma once

//forward declaration, so that we don't need to include SFML in Game Project
namespace sf
{
	class Clock;
}

class Time
{
public:
	Time();
	~Time();

public:
	void Restart();
	const float GetElapsedTimeAsSeconds();

private:
	sf::Clock* m_Clock;
	float m_DeltaTime;

};
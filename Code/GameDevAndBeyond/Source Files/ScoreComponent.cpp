#include "../Header Files/ScoreComponent.h"

ScoreComponent::ScoreComponent()
= default;

ScoreComponent::~ScoreComponent()
= default;

void ScoreComponent::GetScore(int& score)
{
	score = m_Score;
}

void ScoreComponent::SetScore(int score)
{
	m_Score = score;
}

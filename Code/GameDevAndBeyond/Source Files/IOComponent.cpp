#include "../Header Files/IOComponent.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

IOComponent::IOComponent()
= default;

IOComponent::~IOComponent()
= default;

void IOComponent::LoadScoreFromDrive()
{
	std::ifstream input_file("highscore.txt", std::ios::binary);
	input_file.read((char*)&m_HighScoreList, sizeof(m_HighScoreList));
	input_file.close();
	if (m_HighScoreList.size() > 1)
	{
		SortNamedScore(m_HighScoreList);
	}
}

void IOComponent::SaveScoreToDrive()
{
	std::ofstream output_file("highscore.txt", std::ios::binary);
	output_file.write((char*)&m_HighScoreList, sizeof(m_HighScoreList));
	output_file.close();
}

void IOComponent::GetScore(std::string& name, int& score)
{
	if (m_HighScoreList.size() > 1)
	{
		score = m_HighScoreList[0]->score;
		name = m_HighScoreList[0]->name;
	}
}

void IOComponent::SetScore(std::string name, int score)
{
	std::shared_ptr<NamedScore> newScore = std::make_shared<NamedScore>();
	newScore->name = name;
	newScore->score = score;
	//std::string name = m_HighScoreList[0]->name;
	m_HighScoreList.emplace_back(newScore);
	SortNamedScore(m_HighScoreList);
	if (m_HighScoreList.size() > 3)
	{
		m_HighScoreList.resize(3);
	}
}

void IOComponent::GetHighScoreListAsString(std::string highscoreListString)
{
	std::string highscoreAsString = "Score\n";
	for (std::vector<std::shared_ptr<NamedScore>>::iterator highscoreItr = m_HighScoreList.begin(); highscoreItr != m_HighScoreList.end();)
	{
		std::shared_ptr<NamedScore> highscore = *highscoreItr;
		highscoreAsString += highscore->name + "   " + std::to_string(highscore->score) + "\n";
		++highscoreItr;
	}
	highscoreListString = highscoreAsString;
}

void IOComponent::SortNamedScore(std::vector<std::shared_ptr<NamedScore>> vector)
{
	if (m_HighScoreList.size() > 1)
	{
		bool unsorted = true;
		while (unsorted)
		{
			unsorted = false;
			for (size_t iterator = 0; iterator < m_HighScoreList.size() - 1; iterator++)
			{
				if (m_HighScoreList[iterator]->score > m_HighScoreList[iterator + 1]->score)
				{
					std::shared_ptr <NamedScore> temp = m_HighScoreList[iterator];
					m_HighScoreList[iterator] = m_HighScoreList[iterator + 1];
					m_HighScoreList[iterator + 1] = temp;
					unsorted = true;
				}
			}
		}
	}
}

#pragma once
#include <SFMLEngine/Header Files/IComponent.h>
#include <string>
#include <vector>
#include<memory>

struct NamedScore
{
	std::string name;
	int score = 0;
};

class IOComponent : public IComponent
{
public:
	IOComponent();
	~IOComponent();
public:
	void LoadScoreFromDrive();
	void SaveScoreToDrive();
	void GetScore(std::string& name, int& score);
	void SetScore(std::string name, int score);
	void GetHighScoreListAsString(std::string highscoreListString);
	void SortNamedScore(std::vector<std::shared_ptr<NamedScore>> vector);
private:
	std::vector<std::shared_ptr<NamedScore>> m_HighScoreList;
	int m_HighscoreInt = 0;
	std::string m_HighscoreString;
}; 
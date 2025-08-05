#include "HighScore.h"

#include "LevelManager.h"
#include <map>
#include <sstream>
#include <memory>


HighScore::HighScore()
{
}

void HighScore::Init()
{
    Load();
}

int HighScore::GetHighScore(int levelIndex) const
{
    return highScores.at(levelIndex);
}


void HighScore::Load()
{
	XMLDocument doc;
	if (doc.LoadFile(savePath.c_str()) != XML_SUCCESS)
	{
        Create();
		return;
	}

	XMLElement* high = doc.FirstChildElement("highScores");
	if (!high)
	{
		printf("No map element found in highScores file: %s\n", savePath.c_str());
		return;
	}

    XMLElement* levelElement = high->FirstChildElement("level");
    for (int i = 0; levelElement != nullptr; levelElement = levelElement->NextSiblingElement("level"), i++)
    {
        int score;
        if (levelElement->QueryIntAttribute("score", &score) == XML_SUCCESS)
        {
            highScores.push_back(score);
        }
        else
        {
            printf("Failed to read score from level element in highScores file: %s\n", savePath.c_str());
        }
	}
}

void HighScore::Save()
{
    XMLDocument doc;
    XMLElement* high = doc.NewElement("highScores");
    for (int i = 0; i < highScores.size(); i++)
    {
        XMLElement* levelElement = doc.NewElement("level");
        levelElement->SetAttribute("score", highScores[i]);
        high->InsertEndChild(levelElement);
    }
    doc.InsertFirstChild(high);
	doc.SaveFile(savePath.c_str());
}

void HighScore::Create()
{
	highScores.resize(LevelManager::levels.size(), 0); // Initialize with 10 levels, all scores set to 0
    Save();
}

void HighScore::SetHighScore(int levelIndex, int score)
{
    highScores[levelIndex] = score;
    Save();
}

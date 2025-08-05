#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class HighScore
{
private:
	inline static const string savePath = "highscores.xml";
	vector<int> highScores;

private:
	void Load();
	void Create();
	void Save();

public:
	HighScore();
	void Init();

	int GetHighScore(int levelIndex) const;
	void SetHighScore(int levelIndex, int score);

};

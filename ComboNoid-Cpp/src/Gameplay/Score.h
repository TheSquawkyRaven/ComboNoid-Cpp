#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Node/Node.h"
#include "../UI/Text.h"

class Game;
class Gameplay;

class Score : public Node
{
private:
	inline static const int drawLayer = 50;
	inline static const int fontSize = 32;
	inline static SDL_Color textColor{ 255, 255, 255, 255 };

	inline static const int scorePerBallStock = 100;

	Gameplay* gameplay;

	Text* text = nullptr;

public:
	int totalScore = 0;

private:
	void UpdateScore();

public:
	Score(Game* game, Gameplay* gameplay);
	void Init();

	void AddScore(int score);
	void AddBallsStockScore(int ballsStock);

};


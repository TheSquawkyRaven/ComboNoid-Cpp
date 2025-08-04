#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;
class Gameplay;

class Score : public IDestroyable, public IDrawable
{
private:
	inline static const int drawLayer = 50;
	inline static const int fontSize = 32;
	inline static SDL_Color textColor{ 0, 0, 0, 255 };

	Game* game;
	Gameplay* gameplay;

	shared_ptr<TTF_Font> font = nullptr;

public:
	int totalScore = 0;

private:
	void UpdateScore();

public:
	Score(Game* game, Gameplay* gameplay);
	void Init();

	void OnDestroy() override;

	void AddScore(int score);

};


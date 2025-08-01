#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;

class Paddle : public ITransform, public IInput, public IUpdatable, public IDrawable
{
private:
	enum PaddleSize
	{
		SHORT,
		NORMAL,
		LONG
	};

	Game* game;

	SDL_Rect rectShort{ 0, 0, 32, 16 };
	SDL_Rect rectNormal{ 0, 16, 48, 16 };
	SDL_Rect rectLong{ 0, 32, 64, 16 };
	SDL_Rect* currentRect = nullptr;

	bool leftInput = false;
	bool rightInput = false;

	float lLimit = 0;
	float rLimit = 1280;

	float speed = 200;

public:
	Paddle(Game* game);
	void Init();

	void Input(SDL_Event& event) override;

	void Update() override;

	void SetSize(PaddleSize size);

};


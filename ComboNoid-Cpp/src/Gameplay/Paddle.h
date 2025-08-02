#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;

class Ball;

class Paddle : public ITransform, public IInput, public IUpdatable, public IDrawable, public IRectCollidable
{
private:
	enum PaddleSize
	{
		SHORT,
		NORMAL,
		LONG
	};

	Game* game;

	Ball* attachedBall = nullptr;

	SDL_Rect rectShort{ 0, 0, 32, 16 };
	SDL_Rect rectNormal{ 0, 16, 48, 16 };
	SDL_Rect rectLong{ 0, 32, 64, 16 };
	SDL_Rect* currentRect = nullptr;

	Vector2 rectOffset{ 0, 3 };
	Vector2 rectSizeShort{ 32, 10 };
	Vector2 rectSizeNormal{ 48, 10 };
	Vector2 rectSizeLong{ 64, 10 };

	bool leftInput = false;
	bool rightInput = false;
	bool spaceInput = false;

	float lLimit = 0;
	float rLimit = 1280;

	float speed = 200;

public:
	Paddle(Game* game);
	void Init();

	void Input(SDL_Event& event) override;

	void Update() override;

	void SetSize(PaddleSize size);

	void AttachBall(Ball* ball);

	// Returns a -1 to 1 offset based on the hit point on the paddle's center
	float GetHorizontalHitOffset(Vector2 hitPoint);

};


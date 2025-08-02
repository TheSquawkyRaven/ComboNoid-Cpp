#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"
#include "../Vector2.h"

using namespace std;

class Game;

class Ball : public ITransform, public IUpdatable, public IDrawable, public ICircleCollidable
{
private:
	enum BallSize
	{
		NORMAL,
		LARGE
	};

	Game* game;

	SDL_Rect rectNormal{ 16, 0, 16, 16 };
	SDL_Rect rectLarge{ 0, 0, 16, 16 };
	SDL_Rect* currentRect = nullptr;

	float bLimit;

public:
	bool isAttached = false;

	Vector2 direction{ 0, -1 };
	float speed = 200;

	function<void(Ball*)> destroyed;

	Ball(Game* game);
	void Init();

	void Update() override;

	void SetSize(BallSize size);

	void OnCollision(IRectCollidable* rect) override;

};


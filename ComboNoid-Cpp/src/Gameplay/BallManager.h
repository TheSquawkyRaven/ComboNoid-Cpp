#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>
#include <memory>

#include "../Components.h"
#include "Ball.h"

using namespace std;

class Game;

class BallManager
{
private:
	Game* game;
	// Balls in play
	vector<unique_ptr<Ball>> balls;

	int ballsStock = 3;

public:
	function<void(Ball*)> doAttach;

	BallManager(Game* game);
	void Init();

	void CreateBall(bool fromStock);

	// Callback from Ball when it is destroyed (exceeds the bottom limit)
	void OnBallDestroyed(Ball* ball);

};


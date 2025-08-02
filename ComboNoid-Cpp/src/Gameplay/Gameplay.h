#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;

class BallManager;
class Paddle;
class Ball;
class Wall;

class Gameplay : public IUpdatable
{
private:
	Game* game;
	BallManager* ballManager;
	Paddle* paddle;
	Wall* topWall;
	Wall* leftWall;
	Wall* rightWall;

public:
	Gameplay(Game* game);
	void Init();

	void Update() override;

	void OnBallManagerDoAttach(Ball* ball);

};


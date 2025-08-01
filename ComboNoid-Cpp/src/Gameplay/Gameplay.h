#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"
#include "Paddle.h"

class Game;

class Gameplay : public IUpdatable
{
private:
	Game* game;
	Paddle* paddle;

public:
	Gameplay(Game* game);
	void Init();

	void Update() override;

};


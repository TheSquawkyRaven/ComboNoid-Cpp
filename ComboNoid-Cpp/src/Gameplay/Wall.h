#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;

class Wall : public ITransform, public IRectCollidable
{
private:
	Game* game;

public:
	enum Side
	{
		TOP,
		LEFT,
		RIGHT,
	};

	Wall(Game* game);
	void Init(Side side);

};


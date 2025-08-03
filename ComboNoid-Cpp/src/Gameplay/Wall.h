#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;
class Gameplay;

class Wall : public IDestroyable, public IRectCollidable
{
public:
	enum Side
	{
		TOP,
		LEFT,
		RIGHT,
	};

private:
	Game* game;
	Gameplay* gameplay;

public:
	Wall(Game* game, Gameplay* gameplay);
	void Init(Side side);

	void OnDestroy() override;

};


#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Node/NodeRectCollider.h"

class Game;
class Gameplay;

class Wall : public NodeRectCollider
{
public:
	enum Side
	{
		TOP,
		LEFT,
		RIGHT,
	};

public:
	Wall(Game* game);
	void Init(Side side);

};


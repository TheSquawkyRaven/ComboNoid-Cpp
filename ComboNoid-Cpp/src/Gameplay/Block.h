#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"

class Game;

class Block : public ITransform, public IDrawable, public IRectCollidable
{
private:
	Game* game;

	int yColor = 0; // The Y offset of the color in the texture
	int frame = 0; // The frame for animations (x value)
	Vector2 blockSize{ 32, 16 };

public:
	enum Color
	{
		RED,
		GREEN,
		BLUE,
		PURPLE,
		YELLOW,
		ORANGE,
	};

	function<void(Block*)> destroyed;

	Block(Game* game);
	void Init(Color color);

	void SetColor(Color color);

};

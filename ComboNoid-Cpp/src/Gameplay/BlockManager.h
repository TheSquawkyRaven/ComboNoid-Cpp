#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>
#include <memory>

#include "../Components.h"
#include "Block.h"

using namespace std;

class Game;

class BlockManager
{
private:
	Game* game;
	// Balls in play
	int blocksCount = 0;

public:

	BlockManager(Game* game);
	void Init();

	Block* CreateBlock(Block::Color color);

	void OnBlockDestroyed(Block* block);

};


#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <set>
#include <memory>

#include "../Components.h"
#include "../LevelManager.h"
#include "Block.h"

using namespace std;

class Game;
class Gameplay;

class BlockManager : IDestroyable
{
private:
	Game* game;
	Gameplay* gameplay;

	// Blocks in play
	set<Block*> blocks;
	set<Block*> strongBlocks;

public:
	inline static const map <Level::TileType, Block::Color> TileToBlockMap{
		{ Level::RED, Block::RED },
		{ Level::GREEN, Block::GREEN },
		{ Level::BLUE, Block::BLUE },
		{ Level::PURPLE, Block::PURPLE },
		{ Level::YELLOW, Block::YELLOW },
		{ Level::ORANGE, Block::ORANGE },
		{ Level::WOOD, Block::WOOD },
		{ Level::STONE, Block::STONE },
	};

	inline static const map<Block::Color, int> blockScore {
		{ Block::RED, 10 },
		{ Block::GREEN, 20 },
		{ Block::BLUE, 50 },
		{ Block::PURPLE, 80 },
		{ Block::YELLOW, 100 },
		{ Block::ORANGE, 150 },
		{ Block::WOOD, 400 },
		{ Block::STONE, 800 },
	};

private:
	void CreateBlockFromTile(int tile, int data, Vector2& pos);
	void CreateBlock(Block::Color color, Vector2& pos, bool spawnPowerup);

	void OnBlockBroken(Block* block);

public:

	BlockManager(Game* game, Gameplay* gameplay);
	void Init();
	void Destroy(Game* game) override;

	void LoadLevel(Level* level);
};


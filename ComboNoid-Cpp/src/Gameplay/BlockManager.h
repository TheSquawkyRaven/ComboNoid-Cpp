#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <set>
#include <memory>

#include "../Node/Node.h"
#include "../LevelManager.h"
#include "Block.h"
#include "../Clip.h"

using namespace std;

class Game;
class Gameplay;

class BlockManager : public Node
{
private:
	inline static const Vector2 offset{ 16, 8 };

	Gameplay* gameplay;

	unique_ptr<Clip> blockHitClip;
	unique_ptr<Clip> blockBreakClip;


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
	void CreateBlockFromTile(int tile, Vector2& pos);
	void CreateBlock(Block::Color color, Vector2& pos);

	void OnBlockHit(Block* block);
	void OnBlockBroken(Block* block);
	void OnBlockDestroyed(Block* block);

public:

	BlockManager(Game* game, Gameplay* gameplay);

	void LoadLevel(Level* level);
};


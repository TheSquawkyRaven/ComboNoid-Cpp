#include "BlockManager.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Score.h"


BlockManager::BlockManager(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{

}

void BlockManager::Init()
{

}

void BlockManager::LoadLevel(Level* level)
{
	int width = level->tiles->width;
	int x = 0, y = 0;
	for (int i = 0; i < level->tiles->data.size(); i++)
	{
		int tile = level->tiles->data[i];
		if (tile != 0)
		{
			Vector2 pos = Vector2(x * 32, y * 16); // Position blocks in a grid
			CreateBlockFromTile(tile, pos);
		}
		x++;
		if (x >= width)
		{
			x = 0;
			y++;
		}
	}
}

void BlockManager::Destroy(Game* game)
{
	for (auto& block : blocks)
	{
		block->Destroy(game);
	}
	blocks.clear();

	for (auto& block : strongBlocks)
	{
		block->Destroy(game);
	}
	strongBlocks.clear();

	IDestroyable::Destroy(game);
}

void BlockManager::CreateBlockFromTile(int tile, Vector2& pos)
{
    Block::Color color = TileToBlockMap.at(static_cast<Level::TileType>(tile));
    CreateBlock(color, pos);
}

void BlockManager::CreateBlock(Block::Color color, Vector2& pos)
{
	Block* block = new Block(game, gameplay);
	block->broken = [this](Block* b)
	{
		this->OnBlockBroken(b);
	};
	block->Init(color, pos);

	if (block->isStrong)
	{
		strongBlocks.insert(block);
		return;
	}
	blocks.insert(block);
}

void BlockManager::OnBlockBroken(Block* block)
{
	if (!blocks.contains(block))
	{
		strongBlocks.erase(block);
	}
	else
	{
		blocks.erase(block);
	}

	Block::Color color = block->GetColor();
	int score = blockScore.at(color);
	gameplay->score->AddScore(score);

	block->Destroy(game);

	if (blocks.empty())
	{
		// Level copmleted
		printf("All blocks destroyed!\n");
	}
}

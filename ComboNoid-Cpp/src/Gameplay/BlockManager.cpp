#include "BlockManager.h"
#include "../Game.h"


BlockManager::BlockManager(Game* game) : game(game)
{

}

void BlockManager::Init()
{
	for (int x = 0; x < 15; ++x)
	{
		for (int y = 0; y < 15; ++y)
		{
			Block* block = CreateBlock(static_cast<Block::Color>((x + y) % 6)); // Create blocks with different colors
			block->pos = Vector2(x * 32, y * 16); // Position blocks in a grid
		}
	}
}

Block* BlockManager::CreateBlock(Block::Color color)
{
	Block* block = new Block(game);
	block->destroyed = [this](Block* b)
	{
		this->OnBlockDestroyed(b);
	};
	block->Init(color);

	blocksCount++;

	return block;
}

void BlockManager::OnBlockDestroyed(Block* block)
{
	// Manual deletion. The block is not stored or kept track anywhere but this vector, or component vectors in Game.h
	delete block;
	if (blocksCount == 0)
	{
		// Level copmleted
	}
}

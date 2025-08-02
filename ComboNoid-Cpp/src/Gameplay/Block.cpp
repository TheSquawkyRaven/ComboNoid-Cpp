#include "Block.h"
#include "../Game.h"

Block::Block(Game* game) : IDrawable(game), IRectCollidable(game), game(game)
{
}

void Block::Init(Color color)
{
	IDrawable::Init(this);
	IRectCollidable::Init(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/block.png");
	SetTexture(texture);

	size = blockSize;

	SetColor(color);
}

void Block::SetColor(Color color)
{
	int c = color;
	yColor = c * 16;
	CropTexture(0, yColor, blockSize.x, blockSize.y);
}

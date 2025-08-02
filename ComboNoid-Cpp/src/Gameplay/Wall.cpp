#include "Wall.h"
#include "../Game.h"

Wall::Wall(Game* game) : IRectCollidable(game), game(game)
{
}

void Wall::Init(Side side)
{
	IRectCollidable::Init(this);

	switch (side)
	{
		case TOP:
			pos = { 0, -16 };
			size = { game->renderX, 16 };
			break;
		case LEFT:
			pos = { -16, 0 };
			size = { 16, game->renderY };
			break;
		case RIGHT:
			pos = { game->renderX, 0 };
			size = { 16, game->renderY };
			break;
		default:
			printf("Unknown Wall Side %d\n", side);
			return;
	}
}

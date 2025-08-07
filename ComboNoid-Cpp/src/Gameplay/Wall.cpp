#include "Wall.h"
#include "../Game.h"

Wall::Wall(Game* game) : Node(game), NodeRectCollider(game)
{
	layer = Tree::WALL;
}

void Wall::Init(Side side)
{
	switch (side)
	{
		case TOP:
			pos = { game->renderX / 2.0f, -8 };
			size = { game->renderX, 16 };
			break;
		case LEFT:
			pos = { -8, game->renderY / 2.0f };
			size = { 16, game->renderY };
			break;
		case RIGHT:
			pos = { game->renderX + 8, game->renderY / 2.0f };
			size = { 16, game->renderY };
			break;
		default:
			printf("Unknown Wall Side %d\n", side);
			return;
	}
}

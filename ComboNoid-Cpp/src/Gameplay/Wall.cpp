#include "Wall.h"
#include "../Game.h"
#include "Gameplay.h"

Wall::Wall(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Wall::Init(Side side)
{
	gameplay->RegisterWall(this);

	Vector2 pos;

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

	IRectCollidable::PlaceCol(pos.x, pos.y);
}

void Wall::OnDestroy()
{
	gameplay->UnregisterWall(this);
}

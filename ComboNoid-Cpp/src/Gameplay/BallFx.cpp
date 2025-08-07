#include "BallFx.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Paddle.h"
#include "Block.h"

BallFx::BallFx(Game* game) : NodeSprite(game), Node(game)
{

}

void BallFx::Init()
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ballfx.png");
	SetTexture(texture);
	SetVisible(false);
	cropRect = wheelRect;
}

void BallFx::BallUpdate(Vector2& pos)
{
	if (wheelX < 0)
	{
		return;
	}

	rot += game->GetDeltaTime() * wheelRotateSpeed * timeFactor;
	this->pos = pos;
}

void BallFx::SetCombo(int combo)
{
	if (combo <= 0)
	{
		SetVisible(false);
		wheelX = -1;
		return;
	}

	wheelX = 0;
	SetVisible(true);
	for (int i = 0; i < comboTier.size(); i++)
	{
		if (comboTier[i] > combo)
		{
			break;
		}
		wheelX = i;
	}

	cropRect.x = wheelX * wheelRect.w;
}

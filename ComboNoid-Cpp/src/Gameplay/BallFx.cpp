#include "BallFx.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Paddle.h"
#include "Block.h"

BallFx::BallFx(Game* game) : game(game)
{

}

void BallFx::Init()
{
	IDrawable::Register(game);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ballfx.png");
	SetTexture(texture);
	SetVisible(false);
}

void BallFx::OnDestroy()
{
	IDrawable::Unregister(game);
}

void BallFx::BallUpdate(Vector2& pos)
{
	if (wheelX < 0)
	{
		return;
	}

	rotation += game->GetDeltaTime() * wheelRotateSpeed * timeFactor;
	this->pos = pos;
	PlaceTexture(this);
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

	CropTexture(wheelX * wheelRect.w, wheelRect.y, wheelRect.w, wheelRect.h);
}

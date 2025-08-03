#include "Powerup.h"

#include <ctime>
#include <cstdlib>

#include "../Game.h"
#include "Gameplay.h"
#include "BallManager.h"
#include "Paddle.h"

Powerup::Powerup(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Powerup::Init(Type type, Vector2 pos)
{
	IUpdatable::Register(game);
	IDrawable::Register(game);

	gameplay->RegisterPowerup(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/powerups.png");
	SetTexture(texture);

	SetType(type);

	this->pos = pos;
}

void Powerup::SetType(Type type)
{
	this->type = type;

	switch (type)
	{
		case ENLARGE:
			currentRect = &rectEnlarge;
			break;
		case SHRINK:
			currentRect = &rectShrink;
			break;
		case SPLIT_BALL:
			currentRect = &rectSplitBall;
			break;
		case EXTRA_BALL:
			currentRect = &rectExtraBall;
			break;
		case ENLARGE_BALL:
			currentRect = &rectEnlargeBall;
			break;
		case SLOW_BALL:
			currentRect = &rectSlowBall;
			break;
		default:
			printf("Unknown Powerup Type of %d\n", type);
			return;
	}

	IRectCollidable::SetOffset(rectOffset.x, rectOffset.y);
	IRectCollidable::SetSize(rectSize.x, rectSize.y);

	CropTexture(*currentRect);
}

void Powerup::OnDestroy()
{
	IUpdatable::Unregister(game);
	IDrawable::Unregister(game);

	gameplay->UnregisterPowerup(this);
}

void Powerup::Update()
{
	pos.y += game->GetDeltaTime() * speed;
	if (pos.y > game->renderY)
	{
		fellOff(this);
		return;
	}
	PostUpdate();
}

void Powerup::PostUpdate()
{
	PlaceTexture(this);
	PlaceCol(this);
}

void Powerup::OnCollision(IRectCollidable* rect, int _type)
{
	// _type is always Paddle
	Paddle* paddle = static_cast<Paddle*>(rect);

	switch (type)
	{
		case ENLARGE:
			gameplay->paddle->EnlargePaddle();
			break;
		case SHRINK:
			gameplay->paddle->ShrinkPaddle();
			break;
		case SPLIT_BALL:
			gameplay->ballManager->SplitBall();
			break;
		case EXTRA_BALL:
			gameplay->ballManager->GainExtraBall();
			break;
		case ENLARGE_BALL:
			gameplay->ballManager->EnlargeBall();
			break;
		case SLOW_BALL:
			gameplay->ballManager->SlowBall();
			break;
	}
	
	gained(this);
}

Powerup::Type Powerup::RandomType()
{
	int randomValue = Game::RandomIntRange(0, 6);
	return static_cast<Type>(randomValue);
}

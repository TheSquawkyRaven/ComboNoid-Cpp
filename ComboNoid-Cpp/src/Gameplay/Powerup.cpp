#include "Powerup.h"

#include <ctime>
#include <cstdlib>

#include "../Game.h"
#include "Gameplay.h"
#include "BallManager.h"
#include "Paddle.h"

Powerup::Powerup(Game* game, Gameplay* gameplay) : NodeSprite(game), NodeRectCollider(game), Node(game), gameplay(gameplay)
{
	layer = Tree::POWERUP;
}

void Powerup::Init(Type type, Vector2 pos)
{
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
			cropRect = rectEnlarge;
			break;
		case SHRINK:
			cropRect = rectShrink;
			break;
		case SPLIT_BALL:
			cropRect = rectSplitBall;
			break;
		case EXTRA_BALL:
			cropRect = rectExtraBall;
			break;
		case ENLARGE_BALL:
			cropRect = rectEnlargeBall;
			break;
		case SLOW_BALL:
			cropRect = rectSlowBall;
			break;
		default:
			printf("Unknown Powerup Type of %d\n", type);
			return;
	}

	size = rectSize;
}

void Powerup::Update()
{
	pos.y += game->GetDeltaTime() * speed;
	if (pos.y > game->renderY)
	{
		fellOff(this);
		return;
	}
}

void Powerup::OnCollision(NodeRectCollider* rect, Tree::Layer _layer)
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

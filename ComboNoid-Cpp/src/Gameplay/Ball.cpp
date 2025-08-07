#include "Ball.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Paddle.h"
#include "Block.h"

Ball::Ball(Game* game, Gameplay* gameplay) : NodeSprite(game), NodeCircleCollider(game), Node(game), gameplay(gameplay)
{
	ballFx = new BallFx(game);
	hitClip = make_unique<Clip>(game->audioManager, "./assets/audio/ball_hit.wav");

	lLimit = 0;
	rLimit = game->renderX;
	tLimit = 0;
	bLimit = game->renderY;
}

void Ball::Init()
{
	AddChild(ballFx);

	ballFx->Init();

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ball.png");
	SetTexture(texture);

	SetSize(NORMAL);
}

void Ball::Update()
{
	if (isAttached)
	{
		UpdatePowerup();
		PostUpdate();
		return;
	}

	paddleCollisionCooldown -= game->GetDeltaTime();

	Vector2 velocity = direction * speed * game->GetDeltaTime() * timeFactor;
	pos = pos + velocity;

	if (pos.x < lLimit)
	{
		pos.x = lLimit + radius;
	}
	else if (pos.x > rLimit)
	{
		pos.x = rLimit - radius;
	}
	
	if (pos.y < tLimit)
	{
		pos.y = tLimit + radius;
	}
	if (pos.y > bLimit)
	{
		fellOff(this);
		return;
	}

	UpdatePowerup();
	PostUpdate();
}

void Ball::PostUpdate()
{
	ballFx->BallUpdate(pos);
}

void Ball::UpdatePowerup()
{
	if (isSlow)
	{
		slowTimer -= game->GetDeltaTime();
		if (slowTimer < 0)
		{
			color.a = 255;
			ballFx->color.a = color.a;
			isSlow = false;
			timeFactor = 1.0f;
			ballFx->timeFactor = timeFactor;
		}
		else if (slowTimer < slowBlinkTime)
		{
			float f = slowBlinkTime - slowTimer;
			float speed = (1.0f - slowTimer / slowBlinkTime) * slowBlinkSpeed;
			float aFactor = sinf(f * speed) * 0.5f + 0.5f;
			color.a = static_cast<Uint8>(slowBlinkAlpha + ((255 - slowBlinkAlpha) * aFactor));
			ballFx->color.a = color.a;
		}
	}

	if (isBig)
	{
		bigTimer -= game->GetDeltaTime();
		if (bigTimer < 0)
		{
			SetSize(NORMAL);
			isBig = false;
		}
	}
}

void Ball::SetSize(BallSize size)
{
	SDL_Rect* lastRect = currentRect;
	float radius = 0;
	switch (size)
	{
		case NORMAL:
			currentRect = &rectNormal;
			radius = 6;
			isBig = false;
			break;
		case LARGE:
			currentRect = &rectLarge;
			radius = 8;
			isBig = true;
			break;
		default:
			printf("Unknown Ball Size of %d\n", size);
			break;
	}

	this->radius = radius;

	cropRect = *currentRect;
}

void Ball::OnCollision(NodeRectCollider* rect, Tree::Layer layer)
{
	if (isAttached)
	{
		return;
	}

	speed = normalSpeed;

	if (layer == Tree::BLOCK)
	{
		Block* block = static_cast<Block*>(rect);
		if (block->IsBroken())
		{
			// Block is already broken
			return;
		}
		int initialHP = block->GetHP();
		bool blockDestroyed = block->DamageBlock(damage);
		if (blockDestroyed)
		{
			if (isBig)
			{
				// If the ball is big, no need to reflect if it manages to destroy the block
				PostUpdate();
				return;
			}
		}
	}

	hitClip->Play();

	HandleBallReflection(rect->GetDestRect());

	if (layer == Tree::PADDLE)
	{
		// Prevent continuous collision with paddle which can cause some glitches
		if (paddleCollisionCooldown < 0)
		{
			Paddle* paddle = static_cast<Paddle*>(rect);
			paddleCollisionCooldown = paddleCollisionCooldownTime;
			float hitOffset = paddle->GetHorizontalHitOffset(pos);

			// Modify the direction.x based on offset (e.g., curve ball more the farther from center)
			direction.x += hitOffset * paddleHitOffsetFactor;
			direction.Normalize();

			speed = paddleHitSpeed;

			paddle->BallHitPaddle(this);
		}
	}

	// Too horizontal check (checks if the ball is too horizontal)
	// Prevents boring gameplay where the ball just bounces left and right with little to no vertical movement
	float y = abs(direction.y);
	if (y < minVerticalDirection)
	{
		direction.y += (direction.y < 0 ? -1 : 1) * minVerticalDirection;
		direction.Normalize();
	}

	// Also check for too vertical movement otherwise can get stuck in a loop of bouncing up and down
	float x = abs(direction.x);
	if (x < minHorizontalDirection)
	{
		direction.x += (direction.x < 0 ? -1 : 1) * minHorizontalDirection;
		direction.Normalize();
	}

	PostUpdate();
}

// https://claude.ai/public/artifacts/9c251ff5-1f95-4970-8273-5ee462ec654e (Method 2)
void Ball::HandleBallReflection(const SDL_Rect& block)
{
	float expandedLeft = block.x - radius;
	float expandedRight = block.x + block.w + radius;
	float expandedTop = block.y - radius;
	float expandedBottom = block.y + block.h + radius;

	// Check if ball center is inside the expanded rectangle
	if (pos.x >= expandedLeft && pos.x <= expandedRight &&
		pos.y >= expandedTop && pos.y <= expandedBottom)
	{

		Vector2 normal;

		// Determine collision normal based on which expanded edge is closest
		float distToLeft = pos.x - expandedLeft;
		float distToRight = expandedRight - pos.x;
		float distToTop = pos.y - expandedTop;
		float distToBottom = expandedBottom - pos.y;

		float minDist = min({ distToLeft, distToRight, distToTop, distToBottom });

		if (minDist == distToLeft)
		{
			normal = Vector2(-1, 0);
			pos.x = expandedLeft;
		}
		else if (minDist == distToRight)
		{
			normal = Vector2(1, 0);
			pos.x = expandedRight;
		}
		else if (minDist == distToTop)
		{
			normal = Vector2(0, -1);
			pos.y = expandedTop;
		}
		else
		{
			normal = Vector2(0, 1);
			pos.y = expandedBottom;
		}

		direction = direction.Reflect(normal);
	}
}

void Ball::SetComboDamage(int combo, bool paddleHit)
{
	this->damage = combo;
	this->combo = combo;
	if (damage < 1)
	{
		damage = 1;
	}
	ballFx->SetCombo(combo);

	if (combo > 0)
	{
		speed = normalSpeed + comboSpeedIncrease * combo;
	}
}

void Ball::Slow(float time)
{
	slowTimer = time;
	timeFactor = slowFactor;
	ballFx->timeFactor = timeFactor;
	isSlow = true;
}

void Ball::Big(float time)
{
	bigTimer = time;
	SetSize(LARGE);
	isBig = true;
}

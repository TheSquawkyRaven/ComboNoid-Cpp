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
	ballFx->Init();
	AddChild(ballFx);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ball.png");
	SetTexture(texture);

	SetSize(NORMAL);

	centered = true;
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

	float r = radius + 1.0f;
	if (pos.x - r < lLimit)
	{
		pos.x = lLimit + r;
	}
	else if (pos.x + r > rLimit)
	{
		pos.x = rLimit - r;
	}

	if (pos.y - r < tLimit)
	{
		pos.y = tLimit + r;
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

	Vector2 thisPos = pos;
	SDL_Rect to = rect->GetDestRect();
	float closestX = clamp(pos.x, static_cast<float>(to.x), static_cast<float>(to.x + to.w));
	float closestY = clamp(pos.y, static_cast<float>(to.y), static_cast<float>(to.y + to.h));

	Vector2 closestPoint{ closestX, closestY };

	// Vector from closest point to ball center
	Vector2 difference = thisPos - closestPoint;
	float distance = difference.Magnitude();

	if (distance < radius)
	{
		Vector2 normal = GetBallRectNormal(rect);

		// Reflect velocity using normal
		direction = direction.Reflect(normal);

		// Push the ball out of the rect
		float penetration = radius - distance;
		pos = pos + normal * penetration;
	}

	if (layer == Tree::PADDLE)
	{
		// Prevent continuous collision with paddle which can cause some glitches
		if (paddleCollisionCooldown < 0)
		{
			Paddle* paddle = static_cast<Paddle*>(rect);
			paddleCollisionCooldown = paddleCollisionCooldownTime;
			float hitOffset = paddle->GetHorizontalHitOffset(thisPos);

			// Modify the direction.x based on offset (e.g., curve ball more the farther from center)
			direction.x += hitOffset * paddleHitOffsetFactor;
			direction.Normalize();

			paddle->BallHitPaddle(this);
		}
	}

	// Too horizontal check (checks if the ball is too horizontal)
	float y = abs(direction.y);
	if (y < minVerticalDirection)
	{
		direction.y += (direction.y < 0 ? -1 : 1) * minVerticalDirection;
		direction.Normalize();
	}

	PostUpdate();
}

Vector2 Ball::GetBallRectNormal(NodeRectCollider* rect)
{
	Vector2 thisPos = pos;
	SDL_Rect to = rect->GetDestRect();
	float dx = thisPos.x - to.x;
	float dy = thisPos.y - to.y;

	float overlapX = (to.w / 2.0f + radius) - abs(dx);
	float overlapY = (to.h / 2.0f + radius) - abs(dy);

	if (overlapX < overlapY)
	{
		return { dx < 0 ? -1.0f : 1.0f, 0.0f }; // Horizontal bounce
	}
	else
	{
		return { 0.0f, dy < 0 ? -1.0f : 1.0f }; // Vertical bounce
	}
}

void Ball::SetComboDamage(int combo)
{
	this->damage = combo;
	if (damage < 1)
	{
		damage = 1;
	}
	ballFx->SetCombo(combo);
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

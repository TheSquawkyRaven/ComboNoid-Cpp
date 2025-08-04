#include "Ball.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Paddle.h"
#include "Block.h"

Ball::Ball(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
    printf("Ball Created\n");
	lLimit = 0;
	rLimit = game->renderX;
	tLimit = 0;
	bLimit = game->renderY;
}

void Ball::Init()
{
	IUpdatable::Register(game);
	IDrawable::Register(game);

	gameplay->RegisterBall(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ball.png");
	SetTexture(texture);

	SetSize(NORMAL);
}

void Ball::OnDestroy()
{
	IUpdatable::Unregister(game);
	IDrawable::Unregister(game);

	gameplay->UnregisterBall(this);
}

void Ball::Update()
{
	if (isAttached)
	{
		PostUpdate();
		return;
	}

	paddleCollisionCooldown -= game->GetDeltaTime();

	Vector2 velocity = direction * speed * game->GetDeltaTime() * timeFactor;
	pos = pos + velocity;

	if (pos.x < lLimit)
	{
		pos.x = pos.x;
	}
	else if (pos.x + radius > rLimit)
	{
		pos.x = rLimit - radius;
	}

	// Vertical limits adjustments with some of padding
	int padding = currentRect->h / 2.0f - radius + 1.0f;
	if (pos.y + padding < tLimit)
	{
		pos.y = tLimit - padding;
	}
	if (pos.y > bLimit)
	{
		fellOff(this);
		return;
	}

	PostUpdate();
}

void Ball::PostUpdate()
{
	PlaceTexture(this);
	PlaceCol(this);
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
			break;
		case LARGE:
			currentRect = &rectLarge;
			radius = 8;
			break;
		default:
			printf("Unknown Ball Size of %d\n", size);
			break;
	}

	ICircleCollidable::SetOffset(currentRect->w / 2.0f, currentRect->h / 2.0f);
	ICircleCollidable::SetRadius(radius);

	CropTexture(*currentRect);
}

void Ball::OnCollision(IRectCollidable* rect, int type)
{
	// Convert type to CollidedWith
	CollidedWith collidedType = static_cast<CollidedWith>(type);

	if (collidedType == BLOCK)
	{
		Block* block = static_cast<Block*>(rect);
		int initialHP = block->GetHP();
		bool blockDestroyed = block->DamageBlock(damage);
		damage -= 1; // Decrease damage by 1 for each block hit
		damage = max(damage, 1);
		DamageUpdated();
		if (blockDestroyed)
		{
			if (!block->isStrong && damage > initialHP)
			{
				// (for non strong blocks) Damage is higher than its initial hp, skip reflecting, PENETRATE
				PostUpdate();
				return;
			}
		}
	}

	Vector2 thisPos = colPos + colOffset;
	Vector2 rectPos = rect->colPos + rect->colOffset;
	float closestX = clamp(thisPos.x, rectPos.x, rectPos.x + rect->size.x);
	float closestY = clamp(thisPos.y, rectPos.y, rectPos.y + rect->size.y);

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

	if (collidedType == PADDLE)
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

	PostUpdate();
}

Vector2 Ball::GetBallRectNormal(IRectCollidable* rect)
{
	Vector2 thisPos = colPos + colOffset;
	Vector2 rectPos = rect->colPos + rect->colOffset;
	Vector2 rectSize = rect->size;
	float rectCenterX = rectPos.x + rectSize.x / 2.0f;
	float rectCenterY = rectPos.y + rectSize.y / 2.0f;
	float dx = thisPos.x - rectCenterX;
	float dy = thisPos.y - rectCenterY;

	float overlapX = (rectSize.x / 2 + radius) - abs(dx);
	float overlapY = (rectSize.y / 2 + radius) - abs(dy);

	if (overlapX < overlapY)
	{
		return { dx < 0 ? -1.0f : 1.0f, 0.0f }; // Horizontal bounce
	}
	else
	{
		return { 0.0f, dy < 0 ? -1.0f : 1.0f }; // Vertical bounce
	}
}

void Ball::SetComboDamage(int damage)
{
	this->damage = damage;
	DamageUpdated();
}

void Ball::DamageUpdated()
{
	// TODO update texture or number or color or something for the ball
}

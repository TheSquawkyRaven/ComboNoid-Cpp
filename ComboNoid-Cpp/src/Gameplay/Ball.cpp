#include "Ball.h"
#include "../Game.h"
#include "Paddle.h"

Ball::Ball(Game* game) : IUpdatable(game), IDrawable(game), ICircleCollidable(game), game(game)
{
    printf("Ball Created\n");
	bLimit = game->renderY;
}

void Ball::Init()
{
	IDrawable::Init(this);
	ICircleCollidable::Init(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/ball.png");
	SetTexture(texture);

	offset.x = 8;
	offset.y = 8;

	SetSize(NORMAL);
}

void Ball::Update()
{
	if (isAttached)
	{
		return;
	}

	paddleCollisionCooldown -= game->GetDeltaTime();

	Vector2 velocity = direction * speed * game->deltaTime;
	pos = pos + velocity;
	UpdateDestRect();

	if (pos.y > bLimit)
	{
		destroyed(this);
		return;
	}
}

void Ball::SetSize(BallSize size)
{
	SDL_Rect* lastRect = currentRect;
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
	CropTexture(*currentRect);
}

static Vector2 GetAABBNormal(Ball& ball, IRectCollidable* colRect)
{
	Vector2 thisPos = ball.pos + ball.offset;
	Vector2 rectPos = colRect->GetPos();
	Vector2 rectSize = colRect->size;
	float rectCenterX = rectPos.x + rectSize.x / 2.0f;
	float rectCenterY = rectPos.y + rectSize.y / 2.0f;
	float dx = thisPos.x - rectCenterX;
	float dy = thisPos.y - rectCenterY;

	float overlapX = (rectSize.x / 2 + ball.radius) - abs(dx);
	float overlapY = (rectSize.y / 2 + ball.radius) - abs(dy);

	if (overlapX < overlapY)
	{
		return { dx < 0 ? -1.0f : 1.0f, 0.0f }; // Horizontal bounce
	}
	else
	{
		return { 0.0f, dy < 0 ? -1.0f : 1.0f }; // Vertical bounce
	}
}

void Ball::OnCollision(IRectCollidable* rect)
{
	Vector2 thisPos = pos + offset;
	Vector2 rectPos = rect->GetPos();
	float closestX = clamp(thisPos.x, rectPos.x, rectPos.x + rect->size.x);
	float closestY = clamp(thisPos.y, rectPos.y, rectPos.y + rect->size.y);

	Vector2 closestPoint{ closestX, closestY };

	// Vector from closest point to ball center
	Vector2 difference = thisPos - closestPoint;
	float distance = difference.Magnitude();


	if (distance < radius)
	{
		Vector2 normal = GetAABBNormal(*this, rect);

		// Reflect velocity using normal
		direction = direction.Reflect(normal);

		// Push the ball out of the rect
		float penetration = radius - distance;
		pos = pos + normal * penetration;

		// Prevent continuous collision which can cause some glitches
		//if (paddleCollisionCooldown < 0)
		{
			Paddle* paddle = dynamic_cast<Paddle*>(rect);
			if (paddle)
			{
				paddleCollisionCooldown = paddleCollisionCooldownTime;
				float hitOffset = paddle->GetHorizontalHitOffset(thisPos);

				// Modify the direction.x based on offset (e.g., curve ball more the farther from center)
				direction.x += hitOffset;
				direction.Normalize();
			}
		}
	}

	UpdateDestRect();
}

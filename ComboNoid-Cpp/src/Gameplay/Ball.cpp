#include "Ball.h"
#include "../Game.h"

Ball::Ball(Game* game) : IUpdatable(game), IDrawable(game), ICircleCollidable(game), game(game)
{
    printf("Ball Created\n");
	bLimit = game->renderY;
}

void Ball::Init()
{
	static SDL_Texture* texture = game->renderer->LoadTexture("./assets/ball.png");

	IDrawable::Init(this);
	ICircleCollidable::Init(this);

	offset.x = 8;
	offset.y = 8;

	SetTexture(texture);

	SetSize(NORMAL);
}

void Ball::Update()
{
	if (isAttached)
	{
		UpdateDestRect();
		return;
	}
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

void Ball::OnCollision(IRectCollidable* rect)
{
	float x = pos.x + offset.x;
	float y = pos.y + offset.y;
	float rectX = rect->transform->pos.x;
	float rectY = rect->transform->pos.y;

	float closestX = clamp(x, rectX, rectX + rect->size.x);
	float closestY = clamp(y, rectY, rectY + rect->size.y);
	Vector2 closestPoint{ closestX, closestY };

	// Step 2: Vector from closest point to ball center
	Vector2 difference = pos + offset - closestPoint;
	float distance = difference.Magnitude();

	if (distance < radius && distance > 0.0f)
	{
		Vector2 normal = difference.Normalized();

		// Step 3: Reflect velocity using normal
		direction = direction.Reflect(normal);

		// Step 4: Push the ball out of the block
		float penetration = radius - distance;
		pos = pos + normal * penetration;
	}

	UpdateDestRect();
}

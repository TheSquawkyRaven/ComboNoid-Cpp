#include "Paddle.h"
#include "../Game.h"
#include "Ball.h"

Paddle::Paddle(Game* game) : IUpdatable(game), IInput(game), IDrawable(game), IRectCollidable(game), game(game)
{
	rLimit = game->renderX;
}

void Paddle::Init()
{
	IDrawable::Init(this);
	IRectCollidable::Init(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/paddle.png");
	SetTexture(texture);

	SetSize(NORMAL);
}

void Paddle::Input(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool isDown = event.type == SDL_KEYDOWN;
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			leftInput = isDown;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT)
		{
			rightInput = isDown;
		}
		else if (event.key.keysym.sym == SDLK_SPACE)
		{
			spaceInput = isDown;
		}
	}
}

void Paddle::Update()
{
	int xInput = 0;
	xInput += leftInput ? -1 : 0;
	xInput += rightInput ? 1 : 0;

	pos.x += game->deltaTime * xInput * speed;
	UpdateDestRect();

	if (pos.x < lLimit)
	{
		pos.x = lLimit;
	}
	else if (destRect.x + destRect.w > rLimit)
	{
		pos.x = rLimit - destRect.w;
	}
	if (attachedBall != nullptr)
	{
		attachedBall->pos.x = pos.x + (destRect.w - attachedBall->destRect.w) / 2;
		attachedBall->pos.y = pos.y + rectOffset.y -
			attachedBall->destRect.h + // Offset the ball to its size so it aligns on top of the paddle
			attachedBall->destRect.h - attachedBall->radius * 2; // Considers the size (NORMAL or LARGE) of the ball

		if (spaceInput)
		{
			attachedBall->isAttached = false;
			attachedBall = nullptr;
		}
	}
}

void Paddle::SetSize(PaddleSize size)
{
	SDL_Rect* lastRect = currentRect;
	Vector2 rectSize;
	switch (size)
	{
		case SHORT:
			currentRect = &rectShort;
			rectSize = rectSizeShort;
			break;
		case NORMAL:
			currentRect = &rectNormal;
			rectSize = rectSizeNormal;
			break;
		case LONG:
			currentRect = &rectLong;
			rectSize = rectSizeLong;
			break;
		default:
			printf("Unknown Paddle Size of %d\n", size);
			break;
	}
	CropTexture(*currentRect);

	// Collision size
	offset = rectOffset;
	this->size = rectSize;

	// Expand or contract from the center of the paddle
	int w = 0;
	int xOffset = 0;
	if (lastRect != nullptr)
	{
		// Expands/Contracts the paddle to the center
		w = (lastRect->w - currentRect->w) / 2;
	}
	else
	{
		xOffset = game->renderX / 2;
		w = -currentRect->w / 2;
	}
	pos.x = pos.x + xOffset + w;
	pos.y = game->renderY - currentRect->h - 32;
}

void Paddle::AttachBall(Ball* ball)
{
	attachedBall = ball;
	ball->isAttached = true;
	ball->UpdateDestRect();
}

float Paddle::GetHorizontalHitOffset(Vector2 hit)
{
	float centerX = pos.x + size.x / 2;
	float offset = (hit.x - centerX) / (size.x / 2);
	return clamp(offset, -1.0f, 1.0f);
}
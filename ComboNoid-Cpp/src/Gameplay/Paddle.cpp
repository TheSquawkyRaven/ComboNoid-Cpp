#include "Paddle.h"
#include "../Game.h"

Paddle::Paddle(Game* game) : IUpdatable(game), IInput(game), IDrawable(game), game(game)
{
    printf("Paddle Created\n");
	rLimit = game->renderX;
}

void Paddle::Init()
{
	static SDL_Texture* texture = game->renderer->LoadTexture("./assets/paddle.png");

	IDrawable::Init(this);

	SetTexture(texture);

	SetSize(NORMAL);

	printf("Paddle Initialized\n");
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
	}
}

void Paddle::Update()
{
	int xInput = 0;
	xInput += leftInput ? -1 : 0;
	xInput += rightInput ? 1 : 0;

	x += game->deltaTime * xInput * speed;
	UpdateDestRect();

	if (x < lLimit)
	{
		x = lLimit;
	}
	else if (destRect.x + destRect.w > rLimit)
	{
		x = rLimit - destRect.w;
	}
}

void Paddle::SetSize(PaddleSize size)
{
	SDL_Rect* lastRect = currentRect;
	switch (size)
	{
	case SHORT:
		currentRect = &rectShort;
		break;
	case NORMAL:
		currentRect = &rectNormal;
		break;
	case LONG:
		currentRect = &rectLong;
		break;
	default:
		currentRect = &rectNormal;
		break;
	}
	CropTexture(*currentRect);

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
	x = x + xOffset + w;
	y = game->renderY - currentRect->h - 32;
}

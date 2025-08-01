#include "Components.h"
#include "Game.h"

IInput::IInput(Game* game) : game(game)
{
	game->Register(this);
}

IInput::~IInput()
{
	game->Unregister(this);
}

IUpdatable::IUpdatable(Game* game) : game(game)
{
	game->Register(this);
}

IUpdatable::~IUpdatable()
{
	game->Unregister(this);
}

IDrawable::IDrawable(Game* game) : game(game), renderer(game->renderer)
{
	game->Register(this);
}

IDrawable::~IDrawable()
{
	game->Unregister(this);
	SDL_DestroyTexture(texture);
}

void IDrawable::Init(ITransform* transform)
{
	this->transform = transform;
}

void IDrawable::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;
	// Default source and destination rectangles (Draw whole texture)
	SDL_QueryTexture(texture, nullptr, nullptr, &srcRect.w, &srcRect.h);
	srcRect.x = 0;
	srcRect.y = 0;
}

void IDrawable::CropTexture(int x, int y, int w, int h)
{
	srcRect.x = x;
	srcRect.y = y;
	srcRect.w = w;
	srcRect.h = h;
}

void IDrawable::CropTexture(SDL_Rect& rect)
{
	printf("Cropping texture to (%d, %d, %d, %d)\n", rect.x, rect.y, rect.w, rect.h);
	srcRect.x = rect.x;
	srcRect.y = rect.y;
	srcRect.w = rect.w;
	srcRect.h = rect.h;
}

void IDrawable::UpdateDestRect()
{
	destRect.x = transform->x;
	destRect.y = transform->y;
	destRect.w = srcRect.w * transform->scaleX;
	destRect.h = srcRect.h * transform->scaleY;
}

void IDrawable::Draw()
{
	if (texture == nullptr)
	{
		printf("Drawable texture is null!\n");
		return;
	}

	UpdateDestRect();
	renderer->Draw(texture, &srcRect, &destRect);
}
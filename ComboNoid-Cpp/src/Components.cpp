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
	srcRect.x = rect.x;
	srcRect.y = rect.y;
	srcRect.w = rect.w;
	srcRect.h = rect.h;
}

void IDrawable::UpdateDestRect()
{
	destRect.x = transform->pos.x;
	destRect.y = transform->pos.y;
	destRect.w = srcRect.w * transform->scale.x;
	destRect.h = srcRect.h * transform->scale.y;
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

void ICollidable::Init(ITransform* transform)
{
	this->transform = transform;
}

IRectCollidable::IRectCollidable(Game* game)
{
	this->game = game;
	game->Register(this);
}

IRectCollidable::~IRectCollidable()
{
	game->Unregister(this);
}

ICircleCollidable::ICircleCollidable(Game* game)
{
	this->game = game;
	game->Register(this);
}

ICircleCollidable::~ICircleCollidable()
{
	game->Unregister(this);
}

bool ICircleCollidable::CheckCollision(IRectCollidable* colRect)
{
	float x = transform->pos.x + offset.x;
	float y = transform->pos.y + offset.y;
	float rectX = colRect->transform->pos.x;
	float rectY = colRect->transform->pos.y;
	float closestX = clamp(x, rectX, rectX + colRect->size.x);
	float closestY = clamp(y, rectY, rectY + colRect->size.y);

	float dx = x - closestX;
	float dy = y - closestY;

	return (dx * dx + dy * dy) < (radius * radius);
}

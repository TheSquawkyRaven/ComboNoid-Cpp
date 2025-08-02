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
}

void IDrawable::Init(ITransform* transform)
{
	this->transform = transform;
}

void IDrawable::SetTexture(shared_ptr<SDL_Texture> texture)
{
	this->texture = texture;
	// Default source and destination rectangles (Draw whole texture)
	SDL_QueryTexture(texture.get(), nullptr, nullptr, &srcRect.w, &srcRect.h);
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
	renderer->Draw(texture.get(), &srcRect, &destRect);
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

Vector2 IRectCollidable::GetPos()
{
	return transform->pos + offset;
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
	Vector2 thisPos = transform->pos + offset;
	Vector2 rectPos = colRect->GetPos();
	float closestX = clamp(thisPos.x, rectPos.x, rectPos.x + colRect->size.x);
	float closestY = clamp(thisPos.y, rectPos.y, rectPos.y + colRect->size.y);

	float dx = thisPos.x - closestX;
	float dy = thisPos.y - closestY;

	return (dx * dx + dy * dy) < (radius * radius);
}

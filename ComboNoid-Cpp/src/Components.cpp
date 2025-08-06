#include "Components.h"
#include "Game.h"
#include "AudioManager.h"

void IDestroyable::Destroy(Game* game)
{
	if (isDestroyed)
	{
		return;
	}
	game->RegisterDestruction(this);
}

void IInput::Register(Game* game)
{
	game->Register(this);
}

void IInput::Unregister(Game* game)
{
	game->Unregister(this);
}

void IUpdatable::Register(Game* game)
{
	game->Register(this);
}

void IUpdatable::Unregister(Game* game)
{
	game->Unregister(this);
}

void IDrawable::Register(Game* game, int layer)
{
	game->Register(this, layer);
	this->layer = layer;
	renderer = game->renderer;
}

void IDrawable::Unregister(Game* game)
{
	game->Unregister(this, layer);
	this->renderer = nullptr;
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
	CropTexture(rect.x, rect.y, rect.w, rect.h);
}

void IDrawable::PlaceTexture(int x, int y)
{
	destRect.x = x;
	destRect.y = y;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void IDrawable::PlaceTexture(ITransform* transform)
{
	PlaceTextureResized(transform->pos.x, transform->pos.y,
		static_cast<int>(srcRect.w * transform->scale.x),
		static_cast<int>(srcRect.h * transform->scale.y));
	angle = transform->rotation;
}

void IDrawable::PlaceTextureResized(int x, int y, int w, int h)
{
	destRect.x = x;
	destRect.y = y;
	destRect.w = w;
	destRect.h = h;
}

//void IDrawable::SetColor(const SDL_Color& color)
//{
//	SetAlpha(color.a);
//	this->color = color;
//}
//
//void IDrawable::SetAlpha(Uint8 alpha) const
//{
//	renderer->SetTextureAlpha(texture.get(), alpha);
//}

void IDrawable::Draw()
{
	if (!visible)
	{
		return;
	}
	if (texture == nullptr)
	{
		printf("Drawable texture is null!\n");
		return;
	}

	renderer->SetColor(texture.get(), color);
	renderer->Draw(texture.get(), &srcRect, &destRect, angle);
	renderer->ResetColor();
}

void IClip::Register(Game* game, int channel)
{
	audioManager = game->audioManager;
	this->channel = channel;
}

void IClip::Unregister(Game* game)
{
	mix = nullptr;
	channel = -1;
}

void IClip::SetMix(shared_ptr<Mix_Chunk> mix)
{
	this->mix = mix;
}

void IClip::Play()
{
	audioManager->PlayMix(mix, channel);
}

void ICollidable::SetOffset(float x, float y)
{
	colOffset.x = x;
	colOffset.y = y;
}

void ICollidable::PlaceCol(int x, int y)
{
	colPos.x = x;
	colPos.y = y;
}

void ICollidable::PlaceCol(ITransform* transform)
{
	PlaceCol(static_cast<int>(transform->pos.x), static_cast<int>(transform->pos.y));
}

void IRectCollidable::SetSize(float w, float h)
{
	size.x = w;
	size.y = h;
}

bool IRectCollidable::CheckCollision(IRectCollidable* rect)
{
	Vector2 thisPos = colPos + colOffset;
	Vector2 rectPos = rect->colPos + rect->colOffset;
	// Check if the rectangles overlap
	return (thisPos.x < rectPos.x + rect->size.x &&
			thisPos.x + size.x > rectPos.x &&
			thisPos.y < rectPos.y + rect->size.y &&
			thisPos.y + size.y > rectPos.y);
}

void ICircleCollidable::SetRadius(float radius)
{
	this->radius = radius;
}

bool ICircleCollidable::CheckCollision(IRectCollidable* rect)
{
	Vector2 thisPos = colPos + colOffset;
	Vector2 rectPos = rect->colPos + rect->colOffset;
	float closestX = clamp(thisPos.x, rectPos.x, rectPos.x + rect->size.x);
	float closestY = clamp(thisPos.y, rectPos.y, rectPos.y + rect->size.y);

	float dx = thisPos.x - closestX;
	float dy = thisPos.y - closestY;

	return (dx * dx + dy * dy) < (radius * radius);
}

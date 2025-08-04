#include "Background.h"
#include "Game.h"

#include <cmath>


Background::Background(Game* game) : game(game)
{
}

void Background::Init(string backgroundPath)
{
	IUpdatable::Register(game);
	IDrawable::Register(game, drawLayer);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture(backgroundPath);
	SetTexture(texture);

	PlaceTextureResized(0, 0, game->renderX, game->renderY);
}

void Background::Destroy(Game* game)
{
	IDestroyable::Destroy(game);
}

void Background::OnDestroy()
{
	IUpdatable::Unregister(game);
	IDrawable::Unregister(game);
}

void Background::Update()
{
	float s = sinf(game->GetTotalTime() * fogSpeed) * 0.5f + 0.5f; //Normalize to [0, 1]
	fogValue = fogRange.x + s * (fogRange.y - fogRange.x);
}

void Background::Draw()
{
	IDrawable::Draw();
	SDL_SetRenderDrawColor(game->renderer->renderer, fogColor.r, fogColor.g, fogColor.b, fogValue * fogColor.a);
	SDL_RenderFillRect(game->renderer->renderer, &destRect);
	SDL_SetRenderDrawColor(game->renderer->renderer, 255, 255, 255, 255);
}

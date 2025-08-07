#include "Background.h"
#include "Game.h"
#include "Renderer.h"

#include <cmath>

Background::Background(Game* game) : NodeSprite(game), Node(game)
{
}

void Background::Init(string backgroundPath)
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture(backgroundPath);
	SetTexture(texture);

	pos.x = game->renderX / 2.0f;
	pos.y = game->renderY / 2.0f;
	destRect = { 0, 0, static_cast<int>(game->renderX), static_cast<int>(game->renderY) };
}

void Background::Update()
{
	float s = sinf(game->GetTotalTime() * fogSpeed) * 0.5f + 0.5f; //Normalize to [0, 1]
	fogValue = fogRange.x + s * (fogRange.y - fogRange.x);
}

void Background::Draw()
{
	NodeSprite::Draw();

	SDL_SetRenderDrawColor(game->renderer->renderer, fogColor.r, fogColor.g, fogColor.b, fogValue * fogColor.a);
	SDL_RenderFillRect(game->renderer->renderer, &destRect);
	SDL_SetRenderDrawColor(game->renderer->renderer, 255, 255, 255, 255);
}

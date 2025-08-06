#include "NodeSprite.h"
#include "../Game.h"

NodeSprite::NodeSprite(Game* game) : Node(game)
{

}

void NodeSprite::Draw()
{
	if (!texture)
	{
		return;
	}

	int x = static_cast<int>(pos.x);
	int y = static_cast<int>(pos.y);
	int w = static_cast<int>(cropRect.w * scale.x);
	int h = static_cast<int>(cropRect.h * scale.y);
	if (centered)
	{
		destRect.x = x + w / 2;
		destRect.y = y + h / 2;
	}
	else
	{
		destRect.x = x;
		destRect.y = y;
	}
	destRect.w = w;
	destRect.h = h;

	game->renderer->SetColor(texture.get(), color);
	game->renderer->Draw(texture.get(), &cropRect, &destRect, rot);
	game->renderer->ResetColor();
}

void NodeSprite::SetTexture(shared_ptr<SDL_Texture> texture)
{
	if (!texture)
	{
		printf("NodeSprite: SetTexture called with null texture!\n");
		return;
	}
	this->texture = texture;
	SDL_QueryTexture(texture.get(), nullptr, nullptr, &cropRect.w, &cropRect.h);
}

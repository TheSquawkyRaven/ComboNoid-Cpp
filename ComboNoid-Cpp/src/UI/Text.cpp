#include "Text.h"
#include "../Game.h"

#include <cmath>


Text::Text(Game* game) : game(game)
{

}

void Text::Init(const Vector2& center, int drawLayer)
{
	if (drawLayer == INT_MIN)
	{
		drawLayer = defaultDrawLayer;
	}
	IDrawable::Register(game, drawLayer);

	this->center = center;

	font = game->renderer->LoadFont("./assets/upheaval/upheavtt.ttf", fontSize);
	dirty = true;
}

void Text::OnDestroy()
{
	IDrawable::Unregister(game);
}

void Text::SetPos(const Vector2& center)
{
	this->center = center;

	int x = center.x - static_cast<int>(srcRect.w / 2.0f);
	int y = center.y - static_cast<int>(srcRect.h / 2.0f);

	pos.x = x;
	pos.y = y;
	PlaceTexture(this);
}

void Text::Draw()
{
	if (dirty)
	{
		Render();
		dirty = false;
	}

	PlaceTexture(this);

	IDrawable::Draw();
}

void Text::SetText(const string& text)
{
	this->text = text;
	dirty = true;
}

void Text::SetFontSize(int fontSize)
{
	this->fontSize = fontSize;
	dirty = true;
}

void Text::SetColor(const SDL_Color& color)
{
	this->color = color;
	dirty = true;
}

void Text::Render()
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadFontTexture(font.get(), fontSize, text.c_str(), color);
	SetTexture(texture);

	SetPos(center);
}

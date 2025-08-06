#include "Text.h"
#include "../Game.h"

#include <cmath>


Text::Text(Game* game) : NodeSprite(game), Node(game)
{

}

void Text::Init()
{
	font = game->renderer->LoadFont("./assets/upheaval/upheavtt.ttf", fontSize);
	dirty = true;
}

void Text::Draw()
{
	if (dirty)
	{
		Render();
		dirty = false;
	}

	NodeSprite::Draw();
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
}

void Text::Render()
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadFontTexture(font.get(), fontSize, text.c_str());
	SetTexture(texture);
}

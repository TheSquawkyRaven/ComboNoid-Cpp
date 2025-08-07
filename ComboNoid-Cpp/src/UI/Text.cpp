#include "Text.h"
#include "../Game.h"

#include <cmath>


Text::Text(Game* game) : NodeSprite(game), Node(game)
{

}

void Text::Init()
{
	font = game->renderer->LoadFont("./assets/upheaval/upheavtt.ttf", fontSize);
}

void Text::SetText(const string& text, int fontSize, const SDL_Color& color)
{
	this->text = text;
	this->fontSize = fontSize;
	this->color = color;
	Render();
}

void Text::SetText(const string& text)
{
	this->text = text;
	Render();
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

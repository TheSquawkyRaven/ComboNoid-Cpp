#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <memory>

#include "../Node/NodeSprite.h"

using namespace std;

class Text : public NodeSprite
{
private:
	shared_ptr<TTF_Font> font = nullptr;
	int fontSize = 16;
	string text;

private:
	void Render();

public:
	Text(Game* game);
	void Init();

	inline void Draw() override { NodeSprite::Draw(); }

	void SetPos(const Vector2& center);

	void SetText(const string& text, int fontSize, const SDL_Color& color);
	void SetText(const string& text);
	void SetColor(const SDL_Color& color);

};

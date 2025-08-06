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
	bool dirty = false;

	shared_ptr<TTF_Font> font = nullptr;
	int fontSize = 16;
	string text;

private:
	void Render();

public:
	Text(Game* game);
	void Init();

	void Draw() override;

	void SetPos(const Vector2& center);

	void SetText(const string& text);
	void SetFontSize(int fontSize);
	void SetColor(const SDL_Color& color);

};

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"

using namespace std;

class Text : public IDestroyable, public ITransform, public IDrawable
{
private:
	inline static const int drawLayer = 100;

	Game* game;

	shared_ptr<TTF_Font> font = nullptr;
	int fontSize = 16;
	string text;
	SDL_Color color{ 0, 0, 0, 255 };
	Vector2 center;

public:
	Text(Game* game);
	void Init(const Vector2& center);

	void OnDestroy() override;

	void SetPos(const Vector2& center);

	void SetText(const string& text);
	void SetFontSize(int fontSize);
	void SetColor(const SDL_Color& color);

	void Render();

};

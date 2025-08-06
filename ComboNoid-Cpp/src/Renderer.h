#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <map>
#include <memory>

#include "Vector2.h"

using namespace std;

class Renderer
{
private:
	inline static const SDL_Color defaultColor{ 255, 255, 255, 255 };

	SDL_Window* window;

	SDL_Texture* renderTarget;

	map<string, weak_ptr<SDL_Texture>> textureMap;
	map<string, weak_ptr<TTF_Font>> fontMap;

	bool isFullScreen = false;

public:
	int renderX;
	int renderY;

	SDL_Renderer* renderer;

public:
	bool GetIsFullScreen() const { return isFullScreen; }

	Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY);

	void Clear() const;
	void Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect) const;
	void Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, float angle) const;
	void Flush();

	void SetColor(SDL_Texture* texture, const SDL_Color& color) const;
	//void SetTextureAlpha(SDL_Texture* texture, Uint8 alpha) const;
	void ResetColor() const;

	shared_ptr<SDL_Texture> LoadTexture(string path);

	shared_ptr<TTF_Font> LoadFont(string path, int fontSize);
	shared_ptr<SDL_Texture> LoadFontTexture(TTF_Font* font, int fontSize, const char* text) const;

	// Converts window coordinates to render coordinates (like mouse position)
	Vector2 GetWindowCoordToRenderCoord(int windowX, int windowY) const;

	void ToggleFullScreen();

};

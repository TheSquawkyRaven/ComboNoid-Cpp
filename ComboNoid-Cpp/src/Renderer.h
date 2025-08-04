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
public:
	int renderX;
	int renderY;

	SDL_Texture* renderTarget;

	SDL_Window* window;
	SDL_Renderer* renderer;

	map<string, weak_ptr<SDL_Texture>> textureMap;
	map<string, weak_ptr<TTF_Font>> fontMap;

	Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY);

	void Clear();
	void Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Flush();

	shared_ptr<SDL_Texture> LoadTexture(string path);

	shared_ptr<TTF_Font> LoadFont(string path, int fontSize);
	shared_ptr<SDL_Texture> LoadFontTexture(TTF_Font* font, int fontSize, const char* text, SDL_Color& color) const;

	// Converts window coordinates to render coordinates (like mouse position)
	Vector2 GetWindowCoordToRenderCoord(int windowX, int windowY) const;
};

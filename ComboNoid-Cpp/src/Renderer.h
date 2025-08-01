#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>

using namespace std;

class Renderer
{
public:
	SDL_Texture* renderTarget;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY);

	void Clear();
	void Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Flush();

	SDL_Texture* LoadTexture(string path);
};

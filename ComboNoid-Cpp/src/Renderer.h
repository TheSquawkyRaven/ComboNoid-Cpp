#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <map>
#include <memory>

using namespace std;

class Renderer
{
public:
	SDL_Texture* renderTarget;

	SDL_Window* window;
	SDL_Renderer* renderer;

	map<string, weak_ptr<SDL_Texture>> textureMap;

	Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY);

	void Clear();
	void Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);
	void Flush();

	shared_ptr<SDL_Texture> LoadTexture(string path);
};

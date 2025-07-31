//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "Game.h"
#include <memory>

using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	//Create window
	window = SDL_CreateWindow("ComboNoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	// Hardware Accelerated renderer with VSync
	SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (sdlRenderer == nullptr)
	{
		printf("SDL Renderer cannot be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);


	// PNG Loading
	// Attempt to initialize the PNG Loader
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) & imageFlags))
	{
		printf("SDL_image cannot initialize! SDL_IMAGE_ERROR: %s", IMG_GetError());
		return false;
	}

	// Mixer (Audio) Loading
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer cannot initialize! SDL_MIXER_ERROR: %s", Mix_GetError());
		return false;
	}

	// TTF Loading
	if (TTF_Init() == -1)
	{
		printf("SDL_TTF cannot initialize! SDL_TTF_ERROR: %s", TTF_GetError());
		return false;
	}

	//Get window surface
	screenSurface = SDL_GetWindowSurface(window);

	//Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	
	//Update the surface
	SDL_UpdateWindowSurface(window);

    SDL_Event e;
	bool quit = false;

	// Initialization
    std::unique_ptr<Game> game = std::make_unique<Game>();
	game->Init();
	
	while(quit == false)
	{
		quit = game->UpdateInput();
		game->Update();
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}

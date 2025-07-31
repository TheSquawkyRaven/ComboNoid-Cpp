#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <memory>

#include "LevelManager.h"

using namespace std;

class Game
{
private:
	SDL_Event event{};
	shared_ptr<LevelManager> levelManager;

public:
	Game();
	void Init();
	
	void Update();

	// Returns if quit is requested
	bool UpdateInput();

};

class IUpdatable
{
public:
	virtual void Update() = 0;
};

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"
#include "../Background.h"
#include "Button.h"

using namespace std;

class MainMenu : public IDestroyable
{
private:
	Game* game;

	Background* background = nullptr;

	Button* playButton = nullptr;
	Vector2 centerPos{ 0, 0 };

public:
	MainMenu(Game* game);
	void Init();

	void Destroy(Game* game) override;

};

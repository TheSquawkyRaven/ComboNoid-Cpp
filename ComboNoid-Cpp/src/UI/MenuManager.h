#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>
#include <memory>

#include "../Components.h"
#include "../Background.h"
#include "MainMenu.h"
#include "LevelsMenu.h"
#include "../AudioManager.h"

using namespace std;

class MenuManager : public IDestroyable
{
private:
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };
	inline static const int ySpacing = 48;

	inline static const Vector2 centerOffset{ 0, 40 };

	Game* game;

	Background* background = nullptr;

	shared_ptr<Mix_Music> backgroundMusic = nullptr;

	MainMenu* mainMenu = nullptr;
	LevelsMenu* levelsMenu = nullptr;

public:
	MenuManager(Game* game);
	void Init();

	void Destroy(Game* game) override;

	void OpenMainMenu();
	void OpenLevelsMenu();

	void LoadFirstLevel();

};

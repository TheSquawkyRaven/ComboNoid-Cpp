#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>

#include "../Components.h"
#include "../Background.h"
#include "Button.h"

class MenuManager;

using namespace std;

class LevelsMenu : public IDestroyable
{
private:
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };

	inline static const float backButtonY = 280;

	inline static const Vector2 firstLevelButton = Vector2(140, 140);
	inline static const Vector2 levelButtonSpacing = Vector2(50, 50);
	inline static const int columns = 5;

	Game* game;
	MenuManager* menuManager;

	Button* backButton = nullptr;

	vector<Button*> levelButtons;

	Vector2 backButtonPos{};

private:
	void OnBackButtonPressed();

public:
	LevelsMenu(MenuManager* menuManager, Game* game);
	void Init();

	void Destroy(Game* game) override;

	void SetVisible(bool visible);

	void OnLevelPressed(int levelIndex);
	void LaunchFirstLevel();
	void LaunchLevel(int levelIndex);

};

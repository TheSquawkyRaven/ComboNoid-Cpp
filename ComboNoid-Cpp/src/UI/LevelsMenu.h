#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>

#include "../Node/Node.h"
#include "../Background.h"
#include "Button.h"

class MenuManager;

using namespace std;

class LevelsMenu : public Node
{
private:
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };

	inline static const float backButtonY = 280;

	inline static const Vector2 firstLevelButton = Vector2(140, 150);
	inline static const Vector2 levelButtonSpacing = Vector2(50, 50);
	inline static const int columns = 5;

	inline static const SDL_Color highScoreTextColor{ 255, 255, 0, 255 };
	inline static const int highScoreTextY = 100;
	inline static const int highScoreValueYOffset = 10;

	Vector2 highScoreTextPos;

	MenuManager* menuManager;

	Button* backButton = nullptr;

	Text* highScoreText = nullptr;
	Text* highScoreValue = nullptr;

	vector<Button*> levelButtons;

	Vector2 backButtonPos{};

private:
	void OnBackButtonPressed();
	void OnLevelButtonHovered(int levelIndex);
	void OnLevelButtonExitHover();

public:
	LevelsMenu(MenuManager* menuManager, Game* game);
	void Init();

	void OnLevelPressed(int levelIndex);
	void LaunchFirstLevel();

};

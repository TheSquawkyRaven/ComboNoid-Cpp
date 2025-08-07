#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>

#include "../Node/NodeSprite.h"
#include "../Background.h"
#include "Button.h"

using namespace std;

class Gameplay;

class PauseMenu : public NodeSprite
{
private:
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };
	inline static const int ySpacing = 48;

	inline static const Vector2 centerOffset{ 0, 40 };

	inline static const int titleFontSize = 32;
	inline static const int scoreFontSize = 24;
	inline static const SDL_Color titleColor{ 255, 255, 255, 255 };
	inline static const SDL_Color scoreColor{ 255, 255, 0, 255 };
	inline static const Vector2 titleOffset{ 0, -100 };
	inline static const int scoreYOffset = 40;

	Gameplay* gameplay;

	Text* winText = nullptr;
	Text* scoreText = nullptr;
	Text* highScoreText = nullptr;
	Button* nextLevelbutton = nullptr;

	Button* resumeButton = nullptr;
	Button* quitButton = nullptr;

	vector<Button*> buttons;

	Vector2 centerPos{ 0, 0 };

private:
	void OnNextLevelButtonPressed();
	void OnRetryButtonPressed();
	void OnResumeButtonPressed();
	void OnQuitButtonPressed();

public:
	PauseMenu(Game* game, Gameplay* gameplay);
	void Init();

	void GameOver(bool won, int score, int highScore);

};

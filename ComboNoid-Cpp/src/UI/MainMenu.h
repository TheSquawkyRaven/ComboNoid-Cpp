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

class MainMenu : public Node
{
private:
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };
	inline static const int ySpacing = 48;

	inline static const Vector2 centerOffset{ 0, 40 };
	inline static const int titleFontSize = 64;
	inline static const float titleYOffset = -80;
	inline static const SDL_Color titleColor{ 255, 255, 255, 255 };

	inline static const Vector2 optionsOffset{ 160, 40 };
	inline static const SDL_Color optionsColor{ 200, 200, 200, 255 };
	inline static const int optionsFontSize = 16;

	MenuManager* menuManager;

	Text* title = nullptr;
	Button* playButton = nullptr;
	Button* levelsButton = nullptr;
	Button* quitButton = nullptr;
	vector<Button*> mainButtons;

	Button* fullScreenButton = nullptr;
	Button* musicButton = nullptr;
	Button* soundButton = nullptr;
	vector<Button*> optionButtons;

	Vector2 centerPos{ 0, 0 };

private:
	void OnPlayButtonPressed();
	void OnLevelsButtonPressed();
	void OnQuitButtonPressed();
	void OnFullScreenButtonPressed();
	void OnMusicButtonPressed();
	void OnSoundButtonPressed();

public:
	MainMenu(MenuManager* menuManager, Game* game);
	void Init();

};

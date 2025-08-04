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

using namespace std;

class Gameplay;

class PauseMenu : public IDestroyable, public IDrawable
{
private:
	inline static const int backgroundDrawLayer = 99;

	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 0, 0, 0, 255 };
	inline static const int ySpacing = 48;

	inline static const Vector2 centerOffset{ 0, 40 };

	Game* game;
	Gameplay* gameplay;

	Button* resumeButton = nullptr;
	Button* quitButton = nullptr;

	vector<Button*> buttons;

	Vector2 centerPos{ 0, 0 };

private:
	void OnResumeButtonPressed();
	void OnQuitButtonPressed();

public:
	PauseMenu(Game* game, Gameplay* gameplay);
	void Init();

	void Destroy(Game* game) override;
	void OnDestroy() override;

	void SetVisible(bool visible);

};

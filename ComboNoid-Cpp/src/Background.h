#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "Components.h"
#include "Vector2.h"

using namespace std;

class Background : public IDestroyable, public IUpdatable, public IDrawable
{
private:
	inline static const int drawLayer = -100;

	Game* game;

	float fogValue = 0;

public:
	float fogSpeed = 0.25f;
	Vector2 fogRange{ 0.4f, 0.8f };
	SDL_Color fogColor{ 68, 84, 226, 255 };

public:
	Background(Game* game);
	void Init(string backgroundPath = "./assets/background/background.png");

	void OnDestroy() override;
	void Update() override;
	void Draw() override;

};

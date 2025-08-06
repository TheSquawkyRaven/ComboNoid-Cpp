#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>

#include "Node/NodeSprite.h"

using namespace std;

class Background : public NodeSprite
{
private:
	SDL_Rect destRect{};
	float fogValue = 0;

public:
	inline static const float fogSpeed = 0.25f;
	inline static const Vector2 fogRange{ 0.4f, 0.8f };
	inline static const SDL_Color fogColor{ 68, 84, 226, 255 };

public:
	Background(Game* game);
	void Init(string backgroundPath = "./assets/background/background.png");

	void Update() override;
	void Draw() override;

};

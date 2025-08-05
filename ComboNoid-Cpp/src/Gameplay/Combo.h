#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <memory>

#include "../Components.h"
#include "../UI/Text.h"
#include "../Clip.h"

class Game;
class Gameplay;

class Combo : public IDestroyable
{
private:
	inline static const int drawLayer = 50;
	inline static const int fontSize = 16;
	inline static SDL_Color textColor{ 255, 255, 255, 255 };

	Game* game;
	Gameplay* gameplay;

	unique_ptr<Clip> comboIncreaseClip;
	unique_ptr<Clip> comboDecreaseClip;

	Text* text = nullptr;

	int lastCombo = -1;
	int combo = 0;

private:
	void UpdateCombo();

public:
	inline int GetCombo() const { return combo; }

	Combo(Game* game, Gameplay* gameplay);
	void Init();

	void Destroy(Game* game) override;

	// Paddle Flash and Hit
	void PaddleHit();
	// Paddle Flash but hit nothing (Decreases combo)
	void PaddleMiss();
	// Ball hit paddle but not flashing (Does not decrease combo)
	void PaddleMissBall();

	// All balls lost (Resets combo)
	void BallLost();

};


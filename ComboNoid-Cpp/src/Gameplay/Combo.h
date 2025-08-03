#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "../Components.h"

class Game;
class Gameplay;

class Combo : public IDestroyable
{
private:
	Game* game;
	Gameplay* gameplay;

	int lastCombo = 0;
	int combo = 0;

private:
	void UpdateCombo();

public:
	inline int GetCombo() const { return combo; }

	Combo(Game* game, Gameplay* gameplay);
	void Init();

	void OnDestroy() override;

	// Paddle Flash and Hit
	void PaddleHit();
	// Paddle Flash but hit nothing (Decreases combo)
	void PaddleMiss();
	// Ball hit paddle but not flashing (Does not decrease combo)
	void PaddleMissBall();

	// All balls lost (Resets combo)
	void BallLost();

};


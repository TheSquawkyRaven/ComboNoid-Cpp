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

class Combo : public IDestroyable, public IUpdatable
{
private:
	inline static const int drawLayer = 50;
	inline static const int fontSize = 16;
	inline static SDL_Color textColor{ 255, 255, 255, 255 };

	inline static const Vector2 offset{ 50, -20 };

	inline static const float expandTime = 0.3f;
	inline static const float expandRotation = -30.0f;
	inline static const float expandScale = 1.5f;
	inline static const SDL_Color expandColor{ 0, 255, 0, 255 };

	inline static const float lossTime = 0.3f;
	inline static const float lossRotation = 15.0f;
	inline static const float lossScale = 0.8f;
	inline static const SDL_Color lossColor{ 255, 0, 0, 255 };

	Game* game;
	Gameplay* gameplay;

	unique_ptr<Clip> comboIncreaseClip;
	unique_ptr<Clip> comboDecreaseClip;

	Text* text = nullptr;

	int lastCombo = -1;
	int combo = 0;

	float time = 0;
	float rot = 0;
	float sc = 0;
	SDL_Color col{ 0, 0, 0, 255 };

	bool isAnimating = false;
	float timer = 0.5f;

private:
	void UpdateCombo();

public:
	inline int GetCombo() const { return combo; }

	Combo(Game* game, Gameplay* gameplay);
	void Init();

	void Destroy(Game* game) override;
	void OnDestroy() override;
	void Update() override;

	// Paddle Flash and Hit
	void PaddleHit();
	// Paddle Flash but hit nothing (Decreases combo)
	void PaddleMiss();
	// Ball hit paddle but not flashing (Does not decrease combo)
	void PaddleMissBall();

	// All balls lost (Resets combo)
	void BallLost();

};


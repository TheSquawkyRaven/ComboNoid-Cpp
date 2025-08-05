#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <set>
#include <memory>

#include "../Components.h"
#include "Ball.h"
#include "../UI/Text.h"

using namespace std;

class Game;
class Gameplay;

class BallManager : public IDestroyable, public IUpdatable
{
private:
	inline static const int ballCountDrawLayer = 50;
	inline static const int fontSize = 16;
	inline static SDL_Color textColor{ 255, 255, 255, 255 };

	Game* game;
	Gameplay* gameplay;

	Text* text = nullptr;
	Vector2 ballCountPos{};

	// Balls in play
	set<Ball*> balls;

	int ballsStock = 3;

	bool enlarge = false;
	float enlargeTime = 10.0f;
	float enlargeTimer = 0;

	bool slow = false;
	float slowTime = 10.0f;
	float slowTimer = 0;

	float ballSlowFactor = 0.5f;

public:
	function<void(Ball*)> doAttach;

private:
	Ball* CreateBall(bool fromStock);

	void UpdateTimer();

	// Callback from Ball when it is destroyed (exceeds the bottom limit)
	void OnBallFellOff(Ball* ball);

	void EndEnlargeBall();
	void EndSlowBall();

	void UpdateBallCount();

public:
	inline int GetBallsStock() const { return ballsStock; }

	BallManager(Game* game, Gameplay* gameplay);
	void Init();

	void Destroy(Game* game) override;
	void OnDestroy() override;
	void Update() override;

	void GainExtraBall();
	void SplitBall();
	void EnlargeBall();
	void SlowBall();

};


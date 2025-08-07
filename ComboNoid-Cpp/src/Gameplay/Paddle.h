#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <map>
#include <memory>

#include "../Clip.h"

#include "../Node/NodeSprite.h"
#include "../Node/NodeRectCollider.h"

class Game;

class Ball;
class Gameplay;

class Paddle : public NodeSprite, public NodeRectCollider
{
public:
	enum PaddleSize
	{
		SHORT,
		NORMAL,
		LONG
	};

private:
	inline static SDL_Rect rectShort{ 0, 0, 32, 16 };
	inline static SDL_Rect rectNormal{ 0, 16, 48, 16 };
	inline static SDL_Rect rectLong{ 0, 32, 64, 16 };

	inline static const Vector2 rectOffset{ 0, -5 };
	inline static const Vector2 rectSizeShort{ 32, 10 };
	inline static const Vector2 rectSizeNormal{ 48, 10 };
	inline static const Vector2 rectSizeLong{ 64, 10 };

	inline static const float enlargeTime = 10.0f;
	inline static const float shrinkTime = 10.0f;

	// Before 0.1f and after 0.1f of the ball hitting the paddle
	// The paddle accepts the ball as a hit
	inline static const float flashTime = 0.2f;
	inline static const Vector2 flashSizeIncrease{ 8, 8 };
	// Threshold for when after the ball hits the paddle and the paddle starts flashing
	inline static const float flashThreshold = flashTime / 2.0f;

	Gameplay* gameplay;

	unique_ptr<Clip> paddleFlashClip;

	Ball* attachedBall = nullptr;
	SDL_Rect* currentRect = nullptr;

	bool leftInput = false;
	bool rightInput = false;
	bool spaceInput = false;
	bool spaceWasHeld = false;
	bool spaceJustPressed = false;

	float lLimit = 0;
	float rLimit;

	float speed = 240;

	bool sizeChanged = false;
	float sizeChangedTimer = 0;

	bool isFlashing = false;
	float flashTimer = 0;
	bool flashHit = false;

	// Balls that hit the paddle while it is not flashing.
	// Contains the time stamp (game->GetTotalTime()) of when the ball hit the paddle
	map<Ball*, float> pendingFlashBalls;

private:
	void UpdateTimer();
	void UpdateBall();
	void UpdateFlash();

	void SetSize(PaddleSize size);

	void FlashHitBall(Ball* ball);
	void FlashMissBall();
	void FlashMiss();

public:
	Paddle(Game* game, Gameplay* gameplay);
	void Init();

	inline void AddedAsChild(Node* parent) override { NodeRectCollider::AddedAsChild(parent); }
	inline void RemovedAsChild(Node* parent) override { NodeRectCollider::RemovedAsChild(parent); }

	void Input(SDL_Event& event) override;
	void Update() override;
	void Draw() override;

	void AttachBall(Ball* ball);
	
	void BallHitPaddle(Ball* ball);

	// Returns a -1 to 1 offset based on the hit point on the paddle's center
	float GetHorizontalHitOffset(Vector2 hitPoint);

	void EnlargePaddle();
	void ShrinkPaddle();

};


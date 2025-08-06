#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>
#include <memory>
#include <vector>
#include <map>

#include "../Components.h"
#include "../Vector2.h"
#include "../Clip.h"
#include "BallFx.h"

using namespace std;

class Game;
class Gameplay;

class Ball : public IDestroyable, public ITransform, public IUpdatable, public IDrawable, public ICircleCollidable
{
public:
	enum BallSize
	{
		NORMAL,
		LARGE
	};

	enum CollidedWith
	{
		PADDLE = 0,
		BLOCK = 1,
		WALL = 2,
	};

private:

	inline static SDL_Rect rectNormal{ 16, 0, 16, 16 };
	inline static SDL_Rect rectLarge{ 0, 0, 16, 16 };

	// Too low, the ball will be too vertical, too high, the ball will be too horizontal
	inline static const float paddleHitOffsetFactor = 0.75f;
	inline static const float paddleCollisionCooldownTime = 0.01f;

	inline static const float minVerticalDirection = 0.2f;

	inline static const float slowTime = 10.0f;
	inline static const float slowFactor = 0.5f;
	inline static const float slowBlinkTime = 2.5f;
	inline static const float slowBlinkSpeed = 25.0f;
	inline static const float slowBlinkAlpha = 32;
	inline static const float bigTime = 10.0f;

	Game* game;
	Gameplay* gameplay;

	BallFx* ballFx = nullptr;

	unique_ptr<Clip> hitClip;

	SDL_Rect* currentRect = nullptr;

	float lLimit;
	float rLimit;
	float tLimit;
	float bLimit;

	float paddleCollisionCooldown = 0;

	float speed = 200;
	float timeFactor = 1.0f;

	int damage = 1;

	bool isBig = false;
	float bigTimer = 0.0f;

	bool isSlow = false;
	float slowTimer = 0.0f;

public:
	bool isAttached = false;

	Vector2 direction{ 0, -1 };

	function<void(Ball*)> fellOff;

private:
	Vector2 GetBallRectNormal(IRectCollidable* rect);

private:
	void UpdatePowerup();

public:
	inline bool IsBig() const { return isBig; }
	inline float GetBigTimer() const { return bigTimer; }
	inline bool IsSlow() const { return isSlow; }
	inline float GetSlowTimer() const { return slowTimer; }
	inline int GetComboDamage() const { return damage; }

	Ball(Game* game, Gameplay* gameplay);
	void Init();

	void Destroy(Game* game) override;
	void OnDestroy() override;
	void Update() override;
	void PostUpdate();
	void OnCollision(IRectCollidable* rect, int type) override;

	void SetSize(BallSize size);
	void Slow(float time = slowTime);
	void Big(float time = bigTime);

	void SetComboDamage(int damage);

};


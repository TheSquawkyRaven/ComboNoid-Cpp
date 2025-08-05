#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>
#include <memory>

#include "../Components.h"
#include "../Vector2.h"
#include "../Clip.h"

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

	Game* game;
	Gameplay* gameplay;

	unique_ptr<Clip> hitClip;

	SDL_Rect* currentRect = nullptr;

	float lLimit;
	float rLimit;
	float tLimit;
	float bLimit;

	float paddleCollisionCooldown = 0;

	int damage = 1;
	bool isBig = false;

public:
	bool isAttached = false;

	Vector2 direction{ 0, -1 };
	float speed = 200;

	float timeFactor = 1.0f;

	function<void(Ball*)> fellOff;

private:
	Vector2 GetBallRectNormal(IRectCollidable* rect);
	void DamageUpdated();

public:
	inline int GetComboDamage() const { return damage; }

	Ball(Game* game, Gameplay* gameplay);
	void Init();

	void OnDestroy() override;
	void Update() override;
	void PostUpdate();
	void OnCollision(IRectCollidable* rect, int type) override;

	void SetSize(BallSize size);

	void SetComboDamage(int damage);

};


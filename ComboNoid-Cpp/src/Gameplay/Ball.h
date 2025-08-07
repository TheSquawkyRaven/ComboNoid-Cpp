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

#include "../Node/NodeSprite.h"
#include "../Node/NodeCircleCollider.h"
#include "../Vector2.h"
#include "../Clip.h"
#include "BallFx.h"

using namespace std;

class Game;
class Gameplay;

class Ball : public NodeSprite, public NodeCircleCollider
{
public:
	enum BallSize
	{
		NORMAL,
		LARGE
	};

private:

	inline static SDL_Rect rectNormal{ 16, 0, 16, 16 };
	inline static SDL_Rect rectLarge{ 0, 0, 16, 16 };

	// Too low, the ball will be too vertical, too high, the ball will be too horizontal
	inline static const float paddleHitOffsetFactor = 0.75f;
	inline static const float paddleCollisionCooldownTime = 0.05f;

	inline static const float minVerticalDirection = 0.2f;
	inline static const float minHorizontalDirection = 0.01f;

	inline static const float slowTime = 10.0f;
	inline static const float slowFactor = 0.5f;
	inline static const float slowBlinkTime = 2.5f;
	inline static const float slowBlinkSpeed = 25.0f;
	inline static const float slowBlinkAlpha = 32;
	inline static const float bigTime = 10.0f;

	inline static const float normalSpeed = 200.0f;
	inline static const float paddleHitSpeed = 200.0f;
	inline static const float comboSpeedIncrease = 10.0f;

	Gameplay* gameplay;

	BallFx* ballFx = nullptr;

	unique_ptr<Clip> hitClip;

	SDL_Rect* currentRect = nullptr;

	float lLimit;
	float rLimit;
	float tLimit;
	float bLimit;

	float paddleCollisionCooldown = 0;

	float speed = normalSpeed;
	float timeFactor = 1.0f;

	int damage = 1;
	int combo = 0;

	bool isBig = false;
	float bigTimer = 0.0f;

	bool isSlow = false;
	float slowTimer = 0.0f;

public:
	bool isAttached = false;

	Vector2 direction{ 0, -1 };

	function<void(Ball*)> fellOff;

private:
	void HandleBallReflection(const SDL_Rect& block);

private:
	void UpdatePowerup();

public:
	inline bool IsBig() const { return isBig; }
	inline float GetBigTimer() const { return bigTimer; }
	inline bool IsSlow() const { return isSlow; }
	inline float GetSlowTimer() const { return slowTimer; }
	inline int GetComboDamage() const { return combo; }

	Ball(Game* game, Gameplay* gameplay);
	void Init();

	inline void AddedAsChild(Node* parent) override { NodeCircleCollider::AddedAsChild(parent); }
	inline void RemovedAsChild(Node* parent) override { NodeCircleCollider::RemovedAsChild(parent); }
	inline void Draw() override { NodeSprite::Draw(); }

	void Update() override;
	void PostUpdate();
	void OnCollision(NodeRectCollider* rect, Tree::Layer layer) override;

	void SetSize(BallSize size);
	void Slow(float time = slowTime);
	void Big(float time = bigTime);

	void SetComboDamage(int combo, bool paddleHit = false);

};


#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"

class Game;
class Gameplay;

class Powerup : public IDestroyable, public ITransform, public IUpdatable, public IDrawable, public IRectCollidable
{
public:
	enum Type
	{
		ENLARGE,
		SHRINK,
		SPLIT_BALL,
		EXTRA_BALL,
		ENLARGE_BALL,
		SLOW_BALL,
	};

	static Type RandomType();

private:
	inline static SDL_Rect rectEnlarge{ 0, 0, 16, 16 };
	inline static SDL_Rect rectShrink{ 16, 0, 16, 16 };
	inline static SDL_Rect rectSplitBall{ 0, 16, 16, 16 };
	inline static SDL_Rect rectExtraBall{ 16, 16, 16, 16 };
	inline static SDL_Rect rectEnlargeBall{ 0, 32, 16, 16 };
	inline static SDL_Rect rectSlowBall{ 16, 32, 16, 16 };

	inline static const SDL_Rect rectOffset{ 0, 4 };
	inline static const SDL_Rect rectSize{ 16, 9 };

	Game* game;
	Gameplay* gameplay;

	SDL_Rect* currentRect = nullptr;

	Type type = ENLARGE;
	float speed = 100;

public:
	inline static const float spawnChance = 0.1f;

	function<void(Powerup*)> gained;
	function<void(Powerup*)> fellOff;

private:
	void PostUpdate();

public:

	Powerup(Game* game, Gameplay* gameplay);
	void Init(Type type, Vector2 pos);

	void OnDestroy() override;
	void Update() override;
	void OnCollision(IRectCollidable* rect, int type) override;

	void SetType(Type type);

};

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"
#include "PowerupManager.h"

class Game;
class Gameplay;

class Block : public IDestroyable, public ITransform, public IUpdatable, public IDrawable, public IRectCollidable
{
public:
	enum Color
	{
		RED,
		GREEN,
		BLUE,
		PURPLE,
		YELLOW,
		ORANGE,
		WOOD,
		STONE,
	};

private:
	inline static const Vector2 blockSize{ 32, 16 };

	inline static const float breakingTime = 0.5f; // Time to break the block
	inline static const int breakingFrames = 5;
	inline static const int breakingStartFrame = 6; // x position

	inline static const int damageToBreakWood = 8;
	inline static const int damageToBreakStone = 12;

	Game* game;
	Gameplay* gameplay;

	Color color = RED;

	bool breaking = false;
	float breakingTimer = 0;
	
	int hp = 1;

public:
	bool isStrong = false;
	bool spawnPowerup = false;
	function<void(Block*)> broken;

private:
	void SetColor(Color color);
	void SpawnPowerup();

public:
	inline Color GetColor() const { return color; }
	inline int GetHP() const { return hp; }

	Block(Game* game, Gameplay* gameplay);

	// Pos is only set at this Init function
	void Init(Color color, Vector2& pos, bool spawnPowerup);
	void OnDestroy() override;
	void Update() override;

	// Returns remaining damage
	bool DamageBlock(int damage);

};

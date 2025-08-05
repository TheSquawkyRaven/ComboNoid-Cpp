#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>

#include "../Components.h"

class Game;

class BallManager;
class BlockManager;
class PowerupManager;
class Paddle;
class Ball;
class Wall;
class Level;
class Combo;
class Score;
class Background;
class PauseMenu;
class Tutorial;

class Gameplay : public IDestroyable, public IInput
{
private:
	Game* game;

	shared_ptr<Mix_Music> backgroundMusic = nullptr;

	shared_ptr<Level> currentLevel = nullptr;
	int levelIndex = 0;

	Background* background;
	Wall* topWall;
	Wall* leftWall;
	Wall* rightWall;

	Tutorial* tutorial;
	PauseMenu* pauseMenu;

	// Collision Layers (Hard coded for now)
	vector<ICircleCollidable*> balls{}; // Collides with paddles, blocks, walls
	vector<IRectCollidable*> paddles{};
	vector<IRectCollidable*> blocks{};
	vector<IRectCollidable*> walls{};
	vector<IRectCollidable*> powerups{}; // Collides with paddles

	bool isPaused = false;
	bool gameOver = false;

public:

	Paddle* paddle;
	BallManager* ballManager;
	BlockManager* blockManager;
	PowerupManager* powerupManager;
	Combo* combo;
	Score* score;

private:
	template<typename ContainerType, typename ElementType>
	inline void UnregisterVector(ContainerType& container, const ElementType& element)
	{
		auto it = remove(container.begin(), container.end(), element);
		container.erase(it, container.end());
	}

	void OnBallManagerDoAttach(Ball* ball);

public:
	inline void RegisterBall(ICircleCollidable* ball) { balls.push_back(ball); }
	inline void UnregisterBall(ICircleCollidable* ball) { UnregisterVector(balls, ball); }
	inline void RegisterPaddle(IRectCollidable* paddle) { paddles.push_back(paddle); }
	inline void UnregisterPaddle(IRectCollidable* paddle) { UnregisterVector(paddles, paddle); }
	inline void RegisterBlock(IRectCollidable* block) { blocks.push_back(block); }
	inline void UnregisterBlock(IRectCollidable* block) { UnregisterVector(blocks, block); }
	inline void RegisterWall(IRectCollidable* wall) { walls.push_back(wall); }
	inline void UnregisterWall(IRectCollidable* wall) { UnregisterVector(walls, wall); }
	inline void RegisterPowerup(IRectCollidable* powerup) { powerups.push_back(powerup); }
	inline void UnregisterPowerup(IRectCollidable* powerup) { UnregisterVector(powerups, powerup); }

	Gameplay(Game* game);
	void Init(shared_ptr<Level> level, int levelIndex);

	void Destroy(Game* game) override;
	void OnDestroy() override;
	void Input(SDL_Event& event) override;

	void HandleCollisions();

	void Pause(bool paused);

	void GameOver(bool won);

	void RestartLevel();
	void LoadNextLevel();

};


#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>
#include <memory>

#include "../Node/Node.h"

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

class Gameplay : public Node
{
private:
	shared_ptr<Mix_Music> backgroundMusic = nullptr;

	shared_ptr<Level> currentLevel = nullptr;
	int levelIndex = 0;

	Background* background;
	Wall* topWall;
	Wall* leftWall;
	Wall* rightWall;

	Tutorial* tutorial;
	PauseMenu* pauseMenu;

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
	void OnBallManagerDoAttach(Ball* ball);

public:
	Gameplay(Game* game);
	void Init(shared_ptr<Level> level, int levelIndex);

	void Input(SDL_Event& event) override;

	void Pause(bool paused);

	void GameOver(bool won);

	void RestartLevel();
	void LoadNextLevel();

};


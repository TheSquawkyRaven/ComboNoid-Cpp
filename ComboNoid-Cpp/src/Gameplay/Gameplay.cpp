#include "Gameplay.h"
#include "../Game.h"
#include "Paddle.h"
#include "BallManager.h"
#include "BlockManager.h"
#include "PowerupManager.h"
#include "Wall.h"
#include "Ball.h"
#include "Combo.h"
#include "Score.h"
#include "../Background.h"
#include "../UI/PauseMenu.h"
#include "Tutorial.h"


Gameplay::Gameplay(Game* game) : game(game)
{
	background = new Background(game);
	paddle = new Paddle(game, this);
	ballManager = new BallManager(game, this);
	ballManager->doAttach = [this](Ball* ball)
	{
		this->OnBallManagerDoAttach(ball);
	};
	blockManager = new BlockManager(game, this);
	powerupManager = new PowerupManager(game, this);
	combo = new Combo(game, this);
	score = new Score(game, this);

	topWall = new Wall(game, this);
	leftWall = new Wall(game, this);
	rightWall = new Wall(game, this);

	pauseMenu = new PauseMenu(game, this);
	tutorial = new Tutorial(game);
}

void Gameplay::Init(shared_ptr<Level> level, int levelIndex)
{
	IInput::Register(game);

	currentLevel = level;
	this->levelIndex = levelIndex;

	background->Init();

	paddle->Init();
	ballManager->Init();
	blockManager->Init();
	powerupManager->Init();
	combo->Init();
	score->Init();

	topWall->Init(Wall::TOP);
	leftWall->Init(Wall::LEFT);
	rightWall->Init(Wall::RIGHT);

	pauseMenu->Init();
	pauseMenu->SetVisible(false);
	Pause(false);

	blockManager->LoadLevel(level.get());

	tutorial->Init();
	tutorial->LevelLoaded(levelIndex);

	backgroundMusic = game->audioManager->LoadMusic("./assets/audio/game_music.wav");
	game->audioManager->PlayMusic(backgroundMusic);
}

void Gameplay::Input(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			Pause(!isPaused);
		}
	}
}

void Gameplay::Destroy(Game* game)
{
	background->Destroy(game);
	paddle->Destroy(game);
	ballManager->Destroy(game);
	blockManager->Destroy(game);
	powerupManager->Destroy(game);
	combo->Destroy(game);
	score->Destroy(game);

	topWall->Destroy(game);
	leftWall->Destroy(game);
	rightWall->Destroy(game);

	pauseMenu->Destroy(game);
	tutorial->Destroy(game);

	IDestroyable::Destroy(game);
}

void Gameplay::OnDestroy()
{
	IInput::Unregister(game);
}

void Gameplay::HandleCollisions()
{
	// Collisions (We check Circle to Rect only)
	for (auto& ball : balls)
	{
		for (auto& paddle : paddles)
		{
			if (ball->CheckCollision(paddle))
			{
				ball->OnCollision(paddle, Ball::PADDLE);
			}
		}
		for (auto& block : blocks)
		{
			if (ball->CheckCollision(block))
			{
				ball->OnCollision(block, Ball::BLOCK);
			}
		}
		for (auto& wall : walls)
		{
			if (ball->CheckCollision(wall))
			{
				ball->OnCollision(wall, Ball::WALL);
			}
		}
	}
	for (auto& powerup : powerups)
	{
		for (auto& paddle : paddles)
		{
			if (powerup->CheckCollision(paddle))
			{
				powerup->OnCollision(paddle, 0);
			}
		}
	}
}

void Gameplay::OnBallManagerDoAttach(Ball* ball)
{
	paddle->AttachBall(ball);
}

void Gameplay::Pause(bool paused)
{
	if (gameOver)
	{
		return;
	}

	isPaused = paused;
	if (isPaused)
	{
		pauseMenu->SetVisible(true);
		game->timeScale = 0;
		return;
	}
	pauseMenu->SetVisible(false);
	game->timeScale = 1;
}

void Gameplay::GameOver(bool won)
{
	if (gameOver)
	{
		return;
	}

	game->audioManager->StopMusic();

	score->AddBallsStockScore(ballManager->GetBallsStock());
	Pause(true);

	int currentScore = score->totalScore;
	int highScore = game->highScore->GetHighScore(levelIndex);
	pauseMenu->GameOver(won, currentScore, highScore);

	if (won && currentScore > highScore)
	{
		game->highScore->SetHighScore(levelIndex, currentScore);
	}

	gameOver = true;
}

void Gameplay::RestartLevel()
{
	game->TriggerLoadLevel(levelIndex);
}

void Gameplay::LoadNextLevel()
{
	int nextLevel = levelIndex + 1;
	game->TriggerLoadLevel(nextLevel);
}

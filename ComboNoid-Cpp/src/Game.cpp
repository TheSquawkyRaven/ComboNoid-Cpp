#include "Game.h"


Game::Game()
{
	printf("Game Created");
}

void Game::Init()
{
    levelManager = make_shared<LevelManager>();
    levelManager->Init();
}

void Game::Update()
{

}

bool Game::UpdateInput()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}
		// Pass input to the other classes that need them
		// Use e.type == SDL_KEYDOWN or SDL_KEYUP
		// Can be checked using event.key.keysym.sym
	}
	return false;
}

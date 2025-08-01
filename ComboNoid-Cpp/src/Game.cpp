#include "Game.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	this->renderer = new Renderer(window, renderer, renderX, renderY);
}

void Game::Init()
{
    levelManager = make_shared<LevelManager>();
    levelManager->Init();
	gameplay = make_shared<Gameplay>(this);
	gameplay->Init();
}

// Game loop
void Game::Update()
{
	// Updating
	UpdateTime();
	UpdateInput();

	for (auto& updatable : updatables)
	{
		updatable->Update();
	}

	// Rendering
	renderer->Clear();
	for (auto& drawable : drawables)
	{
		drawable->Draw();
	}
	renderer->Flush();
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
		for (auto& input : inputs)
		{
			input->Input(event);
		}
	}
	return false;
}


void Game::UpdateTime()
{
	Uint64 currentTicks = SDL_GetTicks64();
	Uint64 elapsedTicks = currentTicks - lastUpdateTicks;

	realDeltaTime = elapsedTicks / 1000.0f;
	realTotalTime = currentTicks / 1000.0f;

	deltaTime = realDeltaTime * timeScale;
	totalTime += deltaTime;

	lastUpdateTicks = currentTicks;
}
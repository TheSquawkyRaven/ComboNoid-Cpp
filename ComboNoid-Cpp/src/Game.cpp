#include "Game.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	this->renderer = new Renderer(window, renderer, renderX, renderY);
}

void Game::Init()
{
    levelManager = make_shared<LevelManager>();
    levelManager->Init();
	gameplay = make_shared<Gameplay>(this);
	gameplay->Init(levelManager->currentLevel);
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

	HandleDestructions();

	gameplay->HandleCollisions();

	HandleDestructions();

	// Rendering
	renderer->Clear();
	for (auto& drawable : drawables)
	{
		drawable->Draw();
	}
	renderer->Flush();
}

void Game::HandleDestructions()
{
	while (!destructionQueue.empty())
	{
		IDestroyable* obj = destructionQueue.front();
		destructionQueue.pop();

		obj->OnDestroy();
		delete obj;
	}
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
	const float deltaTimeLimit = 1.0f / 60.0f; // If can't reach 60fps, the game will slow down (as well as help with freezes)
	Uint64 currentTicks = SDL_GetTicks64();
	Uint64 elapsedTicks = currentTicks - lastUpdateTicks;

	realDeltaTime = elapsedTicks / 1000.0f;
	realTotalTime = currentTicks / 1000.0f;

	float expectedDeltaTime = clamp(realDeltaTime, 0.0f, deltaTimeLimit);
	deltaTime = expectedDeltaTime * timeScale;

	totalTime += deltaTime;

	lastUpdateTicks = currentTicks;
}

void Game::RegisterDestruction(IDestroyable* obj)
{
	obj->isDestroyed = true;
	destructionQueue.push(obj);
}

int Game::AnimateFrame(int frameCount, float time, float frameTime, bool loop)
{
	int frame = static_cast<int>(time / frameTime);
	if (!loop)
	{
		if (frame >= frameCount)
		{
			return -1;
		}
	}
	frame = frame % frameCount;
	return frame;
}

int Game::RandomIntRange(int min, int max)
{
	return min + (rand() % (max - min));
}

float Game::RandomFloatRange(float min, float max)
{
	float f = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	return min + (max - min) * f;
}

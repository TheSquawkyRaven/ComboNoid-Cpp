#include "Game.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	this->renderer = new Renderer(window, renderer, renderX, renderY);

	levelManager = new LevelManager();
}

void Game::Init()
{
    levelManager->Init();
	TriggerOpenMenu();
}

// Game loop
bool Game::Update()
{
	// Updating
	UpdateTime();
	UpdateInput();

	HandleDestructions();

	for (auto& updatable : updatables)
	{
		updatable->Update();
	}

	HandleDestructions();

	if (gameplay)
	{
		gameplay->HandleCollisions();
	}

	HandleDestructions();

	// Rendering
	UpdateDraw();

	if (quitTriggered)
	{
		return true;
	}
	return false;
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

void Game::UpdateDraw()
{
	// Rendering
	renderer->Clear();
	for (auto& pair : drawablesMap)
	{
		vector<IDrawable*> drawables = pair.second;
		for (auto& drawable : drawables)
		{
			
			drawable->Draw();
		}
	}
	renderer->Flush();
}

void Game::RegisterDestruction(IDestroyable* obj)
{
	obj->isDestroyed = true;
	destructionQueue.push(obj);
}

void Game::TriggerLoadLevel(int levelIndex)
{
	if (menuManager)
	{
		menuManager->Destroy(this);
		menuManager = nullptr;
	}

	if (gameplay)
	{
		// Unload previous gameplay
		gameplay->Destroy(this);
		gameplay = nullptr;
	}

	shared_ptr<Level> loadedLevel = levelManager->LoadLevel(levelIndex);
	if (loadedLevel == nullptr)
	{
		// No more levels to load, return to main menu
		TriggerOpenMenu();
		return;
	}
	// Load gameplay with the level
	gameplay = new Gameplay(this);
	gameplay->Init(loadedLevel, levelIndex);
}

void Game::TriggerOpenMenu()
{
	menuManager = new MenuManager(this);
	menuManager->Init();

	if (gameplay)
	{
		gameplay->Destroy(this);
		gameplay = nullptr;
	}

}

void Game::TriggerQuit()
{
	quitTriggered = true;
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

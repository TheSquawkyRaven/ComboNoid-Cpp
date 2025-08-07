#include "Game.h"


Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	srand(static_cast<unsigned int>(time(nullptr)));
	this->renderer = make_shared<Renderer>(window, renderer, renderX, renderY);
	this->audioManager = make_shared<AudioManager>();

	tree = make_shared<Tree>(this);

	levelManager = make_shared<LevelManager>();
	highScore = make_shared<HighScore>();
}

void Game::Init()
{
	Node* node = new Node(this);
	tree->SetRoot(node);

    levelManager->Init();
	highScore->Init();
	TriggerOpenMenu();
}

// Game loop
bool Game::Update()
{
	// Updating
	UpdateTime();

	UpdateInput();

	tree->Run();

	if (quitTriggered)
	{
		tree->GetRoot()->Destroy(nullptr);
		return true;
	}
	return false;
}

bool Game::UpdateInput()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}
		tree->RunInput(event);
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

void Game::TriggerLoadLevel(int levelIndex)
{
	if (menuManager)
	{
		menuManager->Destroy(tree->GetRoot());
		menuManager = nullptr;
	}

	if (gameplay)
	{
		// Unload previous gameplay
		gameplay->Destroy(tree->GetRoot());
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
	tree->GetRoot()->AddChild(gameplay);
	gameplay->Init(loadedLevel, levelIndex);
}

void Game::TriggerOpenMenu()
{
	menuManager = new MenuManager(this);
	tree->GetRoot()->AddChild(menuManager);
	menuManager->Init();

	if (gameplay)
	{
		gameplay->Destroy(tree->GetRoot());
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

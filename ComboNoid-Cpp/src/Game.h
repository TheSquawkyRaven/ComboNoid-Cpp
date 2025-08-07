#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <memory>
#include <vector>
#include <queue>
#include <string>

#include "Renderer.h"
#include "LevelManager.h"
#include "UI/MenuManager.h"
#include "AudioManager.h"
#include "Gameplay/Gameplay.h"
#include "Gameplay/Block.h"
#include "HighScore.h"
#include "Node/Tree.h"

using namespace std;

class IUpdatable;
class IDrawable;

class Game
{
private:
	SDL_Event event{};

	MenuManager* menuManager;
	Gameplay* gameplay = nullptr;

	Uint64 lastUpdateTicks = 0;

	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	float realDeltaTime = 0.0f;
	float realTotalTime = 0.0f;

	bool quitTriggered = false;

public:
	shared_ptr<Renderer> renderer;
	shared_ptr<AudioManager> audioManager;
	shared_ptr<LevelManager> levelManager;

	shared_ptr<Tree> tree = nullptr;

	shared_ptr<HighScore> highScore = nullptr;

	float timeScale = 1.0f;

	// Rendered screen dimensions
	float renderX = 480;
	float renderY = 360;

private:
	void UpdateTime();

public:
	// Returns -1 if loop is false and the animation exceeds the frame count
	static int AnimateFrame(int frameCount, float time, float frameTime, bool loop);

	static int RandomIntRange(int min, int max);
	static float RandomFloatRange(float min, float max);

	inline float GetDeltaTime() const { return deltaTime; }
	inline float GetTotalTime() const { return totalTime; }
	inline float GetRealDeltaTime() const { return realDeltaTime; }
	inline float GetRealTotalTime() const { return realTotalTime; }

	Game(SDL_Window* window, SDL_Renderer* renderer);
	void Init();

	bool Update();
	// Returns true if quit is requested
	bool UpdateInput();

	void TriggerLoadLevel(int levelIndex);
	void TriggerOpenMenu();

	void TriggerQuit();

};
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
#include "UI/MainMenu.h"
#include "Gameplay/Gameplay.h"
#include "Gameplay/Block.h"

using namespace std;

class IUpdatable;
class IDrawable;

class Game
{
private:
	SDL_Event event{};
	shared_ptr<LevelManager> levelManager;
	shared_ptr<MainMenu> mainMenu;
	shared_ptr<Gameplay> gameplay = nullptr;

	Uint64 lastUpdateTicks = 0;

	vector<IUpdatable*> updatables;
	map<int, vector<IDrawable*>> drawablesMap;
	vector<IInput*> inputs;

	queue<IDestroyable*> destructionQueue;

	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	float realDeltaTime = 0.0f;
	float realTotalTime = 0.0f;

public:
	Renderer* renderer;

	float timeScale = 1.0f;

	// Rendered screen dimensions
	float renderX = 480;
	float renderY = 360;

private:
	template<typename ContainerType, typename ElementType>
	inline void UnregisterVector(ContainerType& container, const ElementType& element)
	{
		auto it = remove(container.begin(), container.end(), element);
		container.erase(it, container.end());
	}

	void UpdateTime();
	void UpdateDraw();

	void HandleDestructions();

public:
	// Returns -1 if loop is false and the animation exceeds the frame count
	static int AnimateFrame(int frameCount, float time, float frameTime, bool loop);

	static int RandomIntRange(int min, int max);
	static float RandomFloatRange(float min, float max);

	inline float GetDeltaTime() const { return deltaTime; }
	inline float GetTotalTime() const { return totalTime; }
	inline float GetRealDeltaTime() const { return realDeltaTime; }
	inline float GetRealTotalTime() const { return realTotalTime; }

	inline void Register(IUpdatable* updatable) { updatables.push_back(updatable); }
	inline void Unregister(IUpdatable* updatable) { UnregisterVector(updatables, updatable); }
	inline void Register(IDrawable* drawable, int layer)
	{
		drawablesMap[layer].push_back(drawable);
	}
	inline void Unregister(IDrawable* drawable, int layer)
	{
		vector<IDrawable*>& drawables = drawablesMap[layer];
		UnregisterVector(drawables, drawable);
	}
	inline void Register(IInput* input) { inputs.push_back(input); }
	inline void Unregister(IInput* input) { UnregisterVector(inputs, input); }

	void RegisterDestruction(IDestroyable* obj);

	Game(SDL_Window* window, SDL_Renderer* renderer);
	void Init();

	void Update();
	// Returns true if quit is requested
	bool UpdateInput();

	// Called from MainMenu, to call LevelManager and then create Gameplay
	void TriggerLoadLevel(string level);

};
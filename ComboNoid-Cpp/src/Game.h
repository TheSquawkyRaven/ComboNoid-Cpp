#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <memory>
#include <vector>

#include "Renderer.h"
#include "LevelManager.h"
#include "Gameplay/Gameplay.h"

using namespace std;

class IUpdatable;
class IDrawable;

class Game
{
private:
	SDL_Event event{};
	shared_ptr<LevelManager> levelManager;

	Uint64 lastUpdateTicks = 0;

	vector<IUpdatable*> updatables;
	vector<IDrawable*> drawables;
	vector<IInput*> inputs;

	// Only support circle to rect collisions for now
	vector<ICircleCollidable*> circleCollidables;
	vector<IRectCollidable*> rectCollidables;

	shared_ptr<Gameplay> gameplay;

	template<typename ContainerType, typename ElementType>
	inline void UnregisterVector(ContainerType& container, const ElementType& element)
	{
		auto it = remove(container.begin(), container.end(), element);
		container.erase(it, container.end());
	}

	void UpdateTime();

public:
	Renderer* renderer;

	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	float realDeltaTime = 0.0f;
	float realTotalTime = 0.0f;
	float timeScale = 1.0f;

	// Rendered screen dimensions
	float renderX = 480;
	float renderY = 360;

	inline float GetDeltaTime() const { return deltaTime; }
	inline float GetTotalTime() const { return totalTime; }
	inline float GetRealDeltaTime() const { return realDeltaTime; }
	inline float GetRealTotalTime() const { return realTotalTime; }

	inline void Register(IUpdatable* updatable) { updatables.push_back(updatable); }
	inline void Unregister(IUpdatable* updatable) { UnregisterVector(updatables, updatable); }
	inline void Register(IDrawable* drawable) { drawables.push_back(drawable); }
	inline void Unregister(IDrawable* drawable) { UnregisterVector(drawables, drawable); }
	inline void Register(IInput* input) { inputs.push_back(input); }
	inline void Unregister(IInput* input) { UnregisterVector(inputs, input); }
	inline void Register(ICircleCollidable* circle) { circleCollidables.push_back(circle); }
	inline void Unregister(ICircleCollidable* circle) { UnregisterVector(circleCollidables, circle); }
	inline void Register(IRectCollidable* rect) { rectCollidables.push_back(rect); }
	inline void Unregister(IRectCollidable* rect) { UnregisterVector(rectCollidables, rect); }

	Game(SDL_Window* window, SDL_Renderer* renderer);
	void Init();
	
	void Update();

	// Returns true if quit is requested
	bool UpdateInput();

};
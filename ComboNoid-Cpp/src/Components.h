#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <algorithm>
#include <memory>

#include "Renderer.h"
#include "Vector2.h"

using namespace std;

class Game;

class ITransform
{
public:
	Vector2 pos{ 0, 0 };
	Vector2 scale{ 1, 1 };
};

class IInput
{
private:
	Game* game;

public:
	IInput(Game* game);
	~IInput();

	virtual void Input(SDL_Event& event) = 0;

};

class IUpdatable
{
private:
	Game* game;

public:
	IUpdatable(Game* game);
	~IUpdatable();

	virtual void Update() = 0;

};

// Init required! Requires ITransform (coupled)
class IDrawable
{
private:
	Game* game;
	Renderer* renderer;
	ITransform* transform = nullptr;

public:
	shared_ptr<SDL_Texture> texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};

	IDrawable(Game* game);
	~IDrawable();

	void Init(ITransform* transform);

	void CropTexture(int x, int y, int w, int h);
	void CropTexture(SDL_Rect& rect);

	// Can be called to update instantly to get the latest destination rect
	void UpdateDestRect();

	void SetTexture(shared_ptr<SDL_Texture> texture);
	virtual void Draw();

};

class ICollidable
{
protected:
	Game* game;
public:
	ITransform* transform = nullptr;

	void Init(ITransform* transform);

};

class IRectCollidable;
class ICircleCollidable;

// Note: Ignores scale!
class IRectCollidable : public ICollidable
{
public:
	Vector2 offset{ 0, 0 };
	Vector2 size{ 0, 0 };

	IRectCollidable(Game* game);
	~IRectCollidable();

	Vector2 GetPos();

	// Callback when a collision with a circle is detected
	virtual void OnCollision(ICircleCollidable* circle) {};

};

// Note: Ignores scale!
// Also, we only support circle to rect collision ONLY
class ICircleCollidable : public ICollidable
{
public:
	// Offset from the pos of transform. Used to determine the center of the circle
	Vector2 offset{ 0, 0 };
	float radius = 0;

	ICircleCollidable(Game* game);
	~ICircleCollidable();

	// Circle to Rect collision
	bool CheckCollision(IRectCollidable* rect);

	// Callback when a collision with a rect is detected
	virtual void OnCollision(IRectCollidable* rect) {};

};

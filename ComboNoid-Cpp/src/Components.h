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

// Any class that requires Register/Unregister should also inherit from this
// As well as classes that manage objects with components
class IDestroyable
{
public:
	// Flag to notify that the object is going to be destroyed
	bool isDestroyed = false;
	// Chain managed objects or handle callbacks here
	virtual void Destroy(Game* game);
	// Called right before the object is actually deallocated
	// Put unregister calls here
	virtual void OnDestroy() {};
};

class ITransform
{
public:
	Vector2 pos{ 0, 0 };
};

class IInput
{
public:
	void Register(Game* game);
	void Unregister(Game* game);

	virtual void Input(SDL_Event& event) {};

};

class IUpdatable
{
public:
	void Register(Game* game);
	void Unregister(Game* game);

	virtual void Update() {};

};

// Requires manual update when moved, using PlaceTexture or PlaceTextureTransform
class IDrawable
{
private:
	Renderer* renderer;

public:
	shared_ptr<SDL_Texture> texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};

	void Register(Game* game);
	void Unregister(Game* game);

	void CropTexture(int x, int y, int w, int h);
	void CropTexture(SDL_Rect& rect);
	void PlaceTexture(int x, int y);
	void PlaceTexture(ITransform* transform);

	void SetTexture(shared_ptr<SDL_Texture> texture);
	virtual void Draw();

};

class ICollidable
{
public:
	Vector2 colPos{ 0, 0 };
	Vector2 colOffset{ 0, 0 };

	void SetOffset(float x, float y);

	void PlaceCol(int x, int y);
	void PlaceCol(ITransform* transform);

};

class IRectCollidable;
class ICircleCollidable;

// Due to hard coded collision layers, these need to be registered and unregistered manually.
class IRectCollidable : public ICollidable
{
public:
	Vector2 size{ 0, 0 }; // Size of the rectangle

	void SetSize(float w, float h);

	// Rect to Rect collision
	bool CheckCollision(IRectCollidable* rect);

	// Callback when a collision with a circle is detected
	// type for passing down the collision type. Use enum for better readability.
	virtual void OnCollision(IRectCollidable* rect, int type) {};

};

// Also, we only support circle to rect collision ONLY
// Due to hard coded collision layers, these need to be registered and unregistered manually.
class ICircleCollidable : public ICollidable
{
public:
	float radius{ 0 };

	void SetRadius(float radius);

	// Circle to Rect collision
	bool CheckCollision(IRectCollidable* rect);

	// Callback when a collision with a rect is detected
	// type for passing down the collision type. Use enum for better readability.
	virtual void OnCollision(IRectCollidable* rect, int type) {};

};

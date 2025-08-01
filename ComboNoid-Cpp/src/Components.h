#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "Renderer.h"

using namespace std;

class Game;

class ITransform
{
public:
	float x = 0;
	float y = 0;
	float scaleX = 1;
	float scaleY = 1;
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
	SDL_Texture* texture = nullptr;
	SDL_Rect srcRect{};
	SDL_Rect destRect{};

	IDrawable(Game* game);
	~IDrawable();

	void Init(ITransform* transform);

	void CropTexture(int x, int y, int w, int h);
	void CropTexture(SDL_Rect& rect);

	// Can be called to update instantly to get the latest destination rect
	void UpdateDestRect();

	void SetTexture(SDL_Texture* texture);
	virtual void Draw();

};

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Components.h"

using namespace std;

class Button : public IDestroyable, public IInput, public IDrawable
{
public:
	enum State
	{
		DISABLED,
		NORMAL,
		HOVER,
		PRESSED
	};
private:
	inline static const int drawLayer = 100;
	inline static SDL_Rect rectNormal{ 0, 0, 32, 16 };
	inline static SDL_Rect rectHover{ 32, 0, 32, 16 };
	inline static SDL_Rect rectPressed{ 64, 0, 32, 16 };

	Game* game;

	SDL_Rect* currentRect = nullptr;
	State state = DISABLED;

	// Only when mouse is pressing the button
	bool leftClickInput = false;

public:
	function<void()> clicked = nullptr;

private:
	static bool PointInRect(int x, int y, SDL_Rect& rect);
	Vector2 GetRendererMousePos(int mouseX, int mouseY);

	void SetState(State state);

public:
	Button(Game* game);
	void Init(Vector2& pos);

	void OnDestroy() override;
	void Input(SDL_Event& event) override;

};

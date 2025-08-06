#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>
#include <string>

#include "../Node/NodeSprite.h"
#include "Text.h"

using namespace std;

class Button : public NodeSprite
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
	inline static const int normalY = 0;
	inline static const int hoverY = 32;
	inline static const int pressedY = 64;
	inline static const int sizeY = 32;
	inline static int sizeXOffset[] = { 192, 144, 80, 0 };

	inline static const int fontSize = 16;

	Text* text = nullptr;

	State state = DISABLED;

	// Only when mouse is pressing the button
	bool leftClickInput = false;
	bool wasJustVisible = false;

	int width = 0;
	int height = 0;
	// 1 - 4
	int sizeX = 4;

public:
	function<void(bool)> hovered = nullptr;
	function<void()> pressed = nullptr;

private:
	static bool PointInRect(int x, int y, SDL_Rect& rect);
	Vector2 GetRendererMousePos(int mouseX, int mouseY);

	void SetState(State state);

public:
	Button(Game* game);
	// Sizes are only 1 to 4
	void Init(int size);

	void Input(SDL_Event& event) override;

	void SetText(const string& textStr, int fontSize = 16, SDL_Color color = { 0, 0, 0, 255 });

	void SetPos(Vector2& center);
	
	void SetVisible(bool visible) override;

};

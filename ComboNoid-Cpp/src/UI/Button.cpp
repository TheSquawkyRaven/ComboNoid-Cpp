#include "Button.h"
#include "../Game.h"

#include <cmath>


Button::Button(Game* game) : game(game)
{

}

void Button::Init(Vector2& pos)
{
	IInput::Register(game);
	IDrawable::Register(game, drawLayer);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/button.png");
	SetTexture(texture);

	SetState(NORMAL);
	PlaceTexture(pos.x - currentRect->w / 2.0f, pos.y - currentRect->h / 2.0f);
}

void Button::OnDestroy()
{
	IInput::Unregister(game);
	IDrawable::Unregister(game, drawLayer);
}

void Button::Input(SDL_Event& event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		Vector2 mousePos = GetRendererMousePos(event.motion.x, event.motion.y);
		if (PointInRect(mousePos.x, mousePos.y, destRect))
		{
			if (leftClickInput)
			{
				SetState(PRESSED);
				return;
			}
			SetState(HOVER);
			return;
		}
		SetState(NORMAL);
		return;
	}
	if (event.button.button == SDL_BUTTON_LEFT)
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			Vector2 mousePos = GetRendererMousePos(event.motion.x, event.motion.y);
			if (PointInRect(mousePos.x, mousePos.y, destRect))
			{
				leftClickInput = true;
				SetState(PRESSED);
				if (clicked)
				{
					clicked();
				}
			}
			return;
		}
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			leftClickInput = false;
			SetState(NORMAL);
		}
	}
}

bool Button::PointInRect(int x, int y, SDL_Rect& rect)
{
	return (x >= rect.x && x <= rect.x + rect.w &&
			y >= rect.y && y <= rect.y + rect.h);
}

Vector2 Button::GetRendererMousePos(int mouseX, int mouseY)
{
	return game->renderer->GetWindowCoordToRenderCoord(mouseX, mouseY);
}

void Button::SetState(State state)
{
	State previousState = this->state;
	if (previousState == state)
	{
		// No change in state
		return;
	}

	this->state = state;
	switch (state)
	{
		case NORMAL:
			currentRect = &rectNormal;
			break;
		case HOVER:
			currentRect = &rectHover;
			break;
		case PRESSED:
			currentRect = &rectPressed;
			break;
	}

	CropTexture(*currentRect);
}

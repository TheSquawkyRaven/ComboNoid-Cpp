#include "Button.h"
#include "../Game.h"

#include <cmath>


Button::Button(Game* game) : game(game)
{
}

void Button::Init(const Vector2& center, int size)
{
	IInput::Register(game);
	IDrawable::Register(game, drawLayer);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/button.png");
	SetTexture(texture);

	sizeX = size;
	SetState(NORMAL);
	PlaceTexture(center.x - width / 2.0f, center.y - height / 2.0f);

	this->center = center;
}

void Button::SetText(const string& text, int fontSize, SDL_Color color)
{
	if (this->text == nullptr)
	{
		this->text = new Text(game);
		this->text->Init(center);
	}
	this->text->SetText(text);
	this->text->SetFontSize(fontSize);
	this->text->SetColor(color);
}

void Button::SetPos(Vector2& center)
{
	PlaceTexture(center.x - width / 2.0f, center.y - height / 2.0f);

	this->center = center;

	if (text != nullptr)
	{
		text->SetPos(center);
	}
}

void Button::Destroy(Game* game)
{
	if (text != nullptr)
	{
		text->Destroy(game);
	}

	IDestroyable::Destroy(game);
}

void Button::OnDestroy()
{
	IInput::Unregister(game);
	IDrawable::Unregister(game);
}

void Button::Input(SDL_Event& event)
{
	if (!GetVisible())
	{
		return;
	}
	if (wasJustVisible)
	{
		wasJustVisible = false;
		return;
	}

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
				if (pressed)
				{
					pressed();
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

	if (previousState != HOVER && state == HOVER)
	{
		if (hovered)
		{
			hovered(true);
		}
	}
	else if (previousState == HOVER && state != HOVER)
	{
		if (hovered)
		{
			hovered(false);
		}
	}

	this->state = state;
	sizeX = clamp(sizeX, 1, 4);
	width = (sizeX + 1) * 16;
	height = sizeY;
	int x = sizeXOffset[sizeX - 1];
	int y = 0;
	switch (state)
	{
		case NORMAL:
			y = normalY;
			break;
		case HOVER:
			y = hoverY;
			break;
		case PRESSED:
			y = pressedY;
			break;
	}

	CropTexture(x, y, width, height);
}

void Button::SetVisible(bool visible)
{
	IDrawable::SetVisible(visible);
	SetState(NORMAL);
	leftClickInput = false;
	wasJustVisible = true;
	if (text != nullptr)
	{
		text->SetVisible(visible);
	}
}

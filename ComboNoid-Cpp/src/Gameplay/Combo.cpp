#include "Combo.h"
#include "../Game.h"
#include "Gameplay.h"

Combo::Combo(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Combo::Init()
{
	IDrawable::Register(game, drawLayer);
	font = game->renderer->LoadFont("./assets/upheaval/upheavtt.ttf", fontSize);

	UpdateText("Combo x0");
	PlaceTexture(300, game->renderY - 40);
}

void Combo::OnDestroy()
{
	IDrawable::Unregister(game);
}

void Combo::UpdateCombo()
{
	if (combo != lastCombo)
	{
		lastCombo = combo;
		if (combo == 0)
		{
			printf("Combo Ended\n");
			return;
		}
		printf("Combo: %d\n", combo);
	}
}

void Combo::UpdateText(const string& text)
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadFontTexture(font.get(), fontSize, text.c_str(), color);
	SetTexture(texture);
	PlaceTexture(300, game->renderY - 40);
}

void Combo::PaddleHit()
{
	combo++;
	UpdateCombo();
}

void Combo::PaddleMiss()
{
	combo--;
	if (combo < 0)
	{
		combo = 0;
	}
	UpdateCombo();
}

void Combo::PaddleMissBall()
{
	combo--;
	if (combo < 0)
	{
		combo = 0;
	}
	UpdateCombo();
}

void Combo::BallLost()
{
	combo = 0;
	UpdateCombo();
}

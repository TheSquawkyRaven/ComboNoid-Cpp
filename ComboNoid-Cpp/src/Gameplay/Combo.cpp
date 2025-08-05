#include "Combo.h"
#include "../Game.h"
#include "Gameplay.h"

Combo::Combo(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
	text = new Text(game);
}

void Combo::Init()
{
	Vector2 center = Vector2(300, game->renderY - 20);
	text->Init(center);
	text->SetFontSize(fontSize);
	text->SetColor(textColor);

	UpdateCombo();
}

void Combo::Destroy(Game* game)
{
	text->Destroy(game);
	IDestroyable::Destroy(game);
}

void Combo::UpdateCombo()
{
	if (combo == lastCombo)
	{
		return;
	}

	lastCombo = combo;
	text->SetText("Combo x" + to_string(combo));
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

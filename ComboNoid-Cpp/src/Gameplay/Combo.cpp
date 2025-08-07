#include "Combo.h"
#include "../Game.h"
#include "Gameplay.h"

Combo::Combo(Game* game, Gameplay* gameplay) : Node(game), gameplay(gameplay)
{
	text = new Text(game);

	comboIncreaseClip = make_unique<Clip>(game->audioManager, "./assets/audio/combo_increase.wav");
	comboDecreaseClip = make_unique<Clip>(game->audioManager, "./assets/audio/combo_decrease.wav");
}

void Combo::Init()
{
	AddChild(text);

	Vector2 center = Vector2(game->renderX / 2 + offset.x, game->renderY + offset.y);
	text->Init();
	text->pos = center;
	text->SetText(" ", fontSize, textColor);

	UpdateCombo();
}

void Combo::Update()
{
	if (!isAnimating)
	{
		return;
	}

	timer -= game->GetDeltaTime();
	float t = timer / time;
	if (t < 0)
	{
		isAnimating = false;
		t = 0;
	}

	t = t * t;

	float scVal = 1.0f + (sc - 1.0f) * t;
	float rotVal = rot * t;

	SDL_Color colorVal = textColor;
	colorVal.r += static_cast<Uint8>((col.r - textColor.r) * t);
	colorVal.g += static_cast<Uint8>((col.g - textColor.g) * t);
	colorVal.b += static_cast<Uint8>((col.b - textColor.b) * t);
	colorVal.a += static_cast<Uint8>((col.a - textColor.a) * t);

	text->SetColor(colorVal);

	text->scale = Vector2(scVal, scVal);
	text->rot = rotVal;
}

void Combo::UpdateCombo()
{
	if (combo == lastCombo)
	{
		return;
	}

	// Ignore last negative combo for animation
	if (lastCombo >= 0)
	{
		isAnimating = true;
		if (combo > lastCombo)
		{
			timer = expandTime;
			time = expandTime;
			rot = expandRotation;
			sc = expandScale;
			col = expandColor;
		}
		else
		{
			timer = lossTime;
			time = lossTime;
			rot = lossRotation;
			sc = lossScale;
			col = lossColor;
		}
	}

	if (combo == 0)
	{
		// Don't show when 0 combo
		text->SetText(" ");
	}
	else
	{
		text->SetText("Combo x" + to_string(combo));
	}

	lastCombo = combo;
}

void Combo::PaddleHit()
{
	combo++;
	UpdateCombo();
	comboIncreaseClip->Play();
}

void Combo::PaddleMiss()
{
	combo--;
	if (combo < 0)
	{
		combo = 0;
	}
	else
	{
		comboDecreaseClip->Play();
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
	else
	{
		comboDecreaseClip->Play();
	}
	UpdateCombo();
}

void Combo::BallLost()
{
	combo = 0;
	UpdateCombo();
}

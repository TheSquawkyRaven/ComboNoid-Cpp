#include "Score.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Combo.h"

Score::Score(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Score::Init()
{
	IDrawable::Register(game, drawLayer);

	font = game->renderer->LoadFont("./assets/upheaval/upheavtt.ttf", fontSize);
	shared_ptr<SDL_Texture> fontTexture = game->renderer->LoadFontTexture(font.get(), fontSize, "0", textColor);
	SetTexture(fontTexture);

	PlaceTexture(400, game->renderY - 40);
}

void Score::OnDestroy()
{
	IDrawable::Unregister(game);
}

void Score::UpdateScore()
{

}

void Score::AddScore(int score)
{
	int combo = gameplay->combo->GetCombo();
	if (combo < 1)
	{
		combo = 1;
	}

	totalScore += score * combo;
	UpdateScore();
}

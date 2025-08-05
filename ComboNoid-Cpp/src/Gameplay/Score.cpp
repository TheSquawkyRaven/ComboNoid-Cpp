#include "Score.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Combo.h"

Score::Score(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
	text = new Text(game);
}

void Score::Init()
{
	Vector2 center(400, game->renderY - fontSize / 2);
	text->Init(center);
	text->SetFontSize(fontSize);
	text->SetColor(textColor);

	UpdateScore();
}

void Score::Destroy(Game* game)
{
	text->Destroy(game);
	IDestroyable::Destroy(game);
}

void Score::UpdateScore()
{
	text->SetText(to_string(totalScore));
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

void Score::AddBallsStockScore(int ballsStock)
{
	AddScore(ballsStock * scorePerBallStock);
}

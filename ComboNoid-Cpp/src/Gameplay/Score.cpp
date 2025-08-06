#include "Score.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Combo.h"

Score::Score(Game* game, Gameplay* gameplay) : Node(game), gameplay(gameplay)
{
	text = new Text(game);
}

void Score::Init()
{
	Vector2 center(400, game->renderY - fontSize / 2);
	AddChild(text);
	text->Init();
	text->pos = center;
	text->SetFontSize(fontSize);
	text->SetColor(textColor);

	UpdateScore();
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

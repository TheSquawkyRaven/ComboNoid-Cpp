#include "PauseMenu.h"
#include "../Game.h"
#include "../Gameplay/Gameplay.h"

#include <cmath>


PauseMenu::PauseMenu(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
	resumeButton = new Button(game);
	quitButton = new Button(game);

	buttons.push_back(resumeButton);
	buttons.push_back(quitButton);

	centerPos.x = game->renderX / 2.0f;
	centerPos.y = game->renderY / 2.0f;
}

void PauseMenu::Init()
{
	IDrawable::Register(game, backgroundDrawLayer);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/background/pause.png");
	SetTexture(texture);
	PlaceTexture(0, 0);

	resumeButton->Init(centerPos, 4);
	resumeButton->InitText("Resume", fontSize, textColor);
	resumeButton->pressed = [this]()
	{
		this->OnResumeButtonPressed();
	};

	quitButton->Init(centerPos, 4);
	quitButton->InitText("Quit", fontSize, textColor);
	quitButton->pressed = [this]()
	{
		this->OnQuitButtonPressed();
	};

	int count = buttons.size();
	Vector2 p = Vector2(centerPos.x + centerOffset.x, centerPos.y + centerOffset.y);
	for (Button* button : buttons)
	{
		button->SetPos(p);
		p.y += ySpacing;
	}

}

void PauseMenu::Destroy(Game* game)
{
	if (winText != nullptr)
	{
		winText->Destroy(game);
		scoreText->Destroy(game);
		highScoreText->Destroy(game);
		nextLevelbutton->Destroy(game);
	}
	resumeButton->Destroy(game);
	quitButton->Destroy(game);

	IDestroyable::Destroy(game);
}

void PauseMenu::OnDestroy()
{
	IDrawable::Unregister(game);
}

void PauseMenu::OnNextLevelButtonPressed()
{
	gameplay->LoadNextLevel();
}

void PauseMenu::OnRetryButtonPressed()
{
	gameplay->RestartLevel();
}

void PauseMenu::OnResumeButtonPressed()
{
	gameplay->Pause(false);
}

void PauseMenu::OnQuitButtonPressed()
{
	game->TriggerOpenMenu();
}

void PauseMenu::SetVisible(bool visible)
{
	IDrawable::SetVisible(visible);
	for (Button* button : buttons)
	{
		button->SetVisible(visible);
	}
}

void PauseMenu::GameOver(bool won, int score, int highScore)
{
	winText = new Text(game);
	scoreText = new Text(game);
	highScoreText = new Text(game);
	nextLevelbutton = new Button(game);

	winText->SetFontSize(titleFontSize);
	winText->SetColor(titleColor);

	scoreText->SetFontSize(scoreFontSize);
	scoreText->SetColor(scoreColor);
	highScoreText->SetFontSize(scoreFontSize);
	highScoreText->SetColor(scoreColor);

	Vector2 p = centerPos;
	p.x += titleOffset.x;
	p.y += titleOffset.y;

	winText->Init(p);
	if (won)
	{
		winText->SetText("You Win!");
	}
	else
	{
		winText->SetText("You Lose");
	}

	p.y += scoreYOffset;
	scoreText->Init(p);
	scoreText->SetText("Score: " + to_string(score));

	p.y += scoreYOffset;
	highScoreText->Init(p);
	if (score > highScore)
	{
		highScoreText->SetText("NEW HIGH SCORE!");
	}
	else
	{
		highScoreText->SetText("High Score: " + to_string(highScore));
	}

	nextLevelbutton->Init(centerPos, 4);
	if (won)
	{
		nextLevelbutton->InitText("Next", fontSize, textColor);
		nextLevelbutton->pressed = [this]()
		{
			this->OnNextLevelButtonPressed();
		};
	}
	else
	{
		nextLevelbutton->InitText("Retry", fontSize, textColor);
		nextLevelbutton->pressed = [this]()
		{
			this->OnRetryButtonPressed();
		};
	}
	p = Vector2(centerPos.x + centerOffset.x, centerPos.y + centerOffset.y);
	nextLevelbutton->SetPos(p);

	resumeButton->SetVisible(false);
}


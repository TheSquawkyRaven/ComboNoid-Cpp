#include "PauseMenu.h"
#include "../Game.h"
#include "../Gameplay/Gameplay.h"

#include <cmath>


PauseMenu::PauseMenu(Game* game, Gameplay* gameplay) : NodeSprite(game), Node(game), gameplay(gameplay)
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
	AddChild(resumeButton);
	AddChild(quitButton);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/background/pause.png");
	SetTexture(texture);
	pos.x = game->renderX / 2.0f;
	pos.y = game->renderY / 2.0f;

	resumeButton->Init(4);
	resumeButton->pos = centerPos;
	resumeButton->SetText("Resume", fontSize, textColor);
	resumeButton->pressed = [this]()
	{
		this->OnResumeButtonPressed();
	};

	quitButton->Init(4);
	quitButton->pos = centerPos;
	quitButton->SetText("Quit", fontSize, textColor);
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

void PauseMenu::GameOver(bool won, int score, int highScore)
{
	winText = new Text(game);
	scoreText = new Text(game);
	highScoreText = new Text(game);
	nextLevelbutton = new Button(game);

	AddChild(winText);
	AddChild(scoreText);
	AddChild(highScoreText);
	AddChild(nextLevelbutton);

	Vector2 p = centerPos;
	p.x += titleOffset.x;
	p.y += titleOffset.y;

	winText->Init();
	winText->pos = p;
	if (won)
	{
		winText->SetText("You Win!", titleFontSize, titleColor);
	}
	else
	{
		winText->SetText("You Lose", titleFontSize, titleColor);
	}

	p.y += scoreYOffset;
	scoreText->Init();
	scoreText->pos = p;
	if (won)
	{
		scoreText->SetText("Score: " + to_string(score), scoreFontSize, scoreColor);
	}
	else
	{
		// Don't show score if the player lost
		scoreText->SetText(" ", scoreFontSize, scoreColor);
	}

	p.y += scoreYOffset;
	highScoreText->Init();
	highScoreText->pos = p;

	if (won && score > highScore)
	{
		highScoreText->SetText("NEW HIGH SCORE!", scoreFontSize, scoreColor);
	}
	else
	{
		highScoreText->SetText("High Score: " + to_string(highScore), scoreFontSize, scoreColor);
	}

	nextLevelbutton->Init(4);
	nextLevelbutton->pos = centerPos;
	if (won)
	{
		nextLevelbutton->SetText("Next", fontSize, textColor);
		nextLevelbutton->pressed = [this]()
		{
			this->OnNextLevelButtonPressed();
		};
	}
	else
	{
		nextLevelbutton->SetText("Retry", fontSize, textColor);
		nextLevelbutton->pressed = [this]()
		{
			this->OnRetryButtonPressed();
		};
	}
	p = Vector2(centerPos.x + centerOffset.x, centerPos.y + centerOffset.y);
	nextLevelbutton->SetPos(p);

	resumeButton->SetVisible(false);
}


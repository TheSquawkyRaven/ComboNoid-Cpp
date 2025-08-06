#include "LevelsMenu.h"
#include "../Game.h"
#include "MenuManager.h"

#include <cmath>


LevelsMenu::LevelsMenu(MenuManager* menuManager, Game* game) : game(game), menuManager(menuManager)
{
	backButton = new Button(game);

	highScoreText = new Text(game);
	highScoreValue = new Text(game);

	backButtonPos.x = game->renderX / 2.0f;
	backButtonPos.y = backButtonY;

	highScoreTextPos.x = game->renderX / 2.0f;
	highScoreTextPos.y = highScoreTextY;
}

void LevelsMenu::Init()
{
	Vector2 hPos = highScoreTextPos;
	highScoreText->Init(hPos);
	highScoreText->SetFontSize(fontSize);
	highScoreText->SetColor(highScoreTextColor);
	highScoreText->SetText("High Score:");

	hPos.y += highScoreValueYOffset;
	highScoreValue->Init(hPos);
	highScoreValue->SetFontSize(fontSize);
	highScoreValue->SetColor(highScoreTextColor);
	highScoreValue->SetText(" ");

	highScoreText->SetVisible(false);
	highScoreValue->SetVisible(false);

	backButton->Init(backButtonPos, 4);
	backButton->SetText("Back", fontSize, textColor);
	backButton->pressed = [this]()
	{
		this->OnBackButtonPressed();
	};

	int x = 0;
	int y = 0;
	Vector2 p;
	int c = 1;
	for (int i = 0; i < LevelManager::levels.size(); i++)
	{
		if (x == columns)
		{
			x = 0;
			y++;
		}

		p.x = firstLevelButton.x + levelButtonSpacing.x * x;
		p.y = firstLevelButton.y + levelButtonSpacing.y * y;

		Button* button = new Button(game);
		button->Init(p, 1);
		button->SetText(to_string(c), fontSize, textColor);
		button->pressed = [this, i]()
		{
			this->OnLevelPressed(i);
		};
		button->hovered = [this, i](bool hover)
		{
			if (hover)
			{
				this->OnLevelButtonHovered(i);
			}
			else
			{
				this->OnLevelButtonExitHover();
			}
		};

		levelButtons.push_back(button);
		x++;
		c++;
	}
}

void LevelsMenu::Destroy(Game* game)
{
	backButton->Destroy(game);
	for (Button* button : levelButtons)
	{
		button->Destroy(game);
	}
	levelButtons.clear();

	highScoreText->Destroy(game);
	highScoreValue->Destroy(game);

	IDestroyable::Destroy(game);
}

void LevelsMenu::OnBackButtonPressed()
{
	menuManager->OpenMainMenu();
}

void LevelsMenu::SetVisible(bool visible)
{
	backButton->SetVisible(visible);
	for (Button* button : levelButtons)
	{
		button->SetVisible(visible);
	}
	highScoreText->SetVisible(false);
	highScoreValue->SetVisible(false);
}

void LevelsMenu::OnLevelPressed(int levelIndex)
{
	game->TriggerLoadLevel(levelIndex);
}

void LevelsMenu::LaunchFirstLevel()
{
	game->TriggerLoadLevel(0);
}

void LevelsMenu::OnLevelButtonHovered(int levelIndex)
{
	int highScore = game->highScore->GetHighScore(levelIndex);
	if (highScore == 0)
	{
		highScoreText->SetVisible(false);
		highScoreValue->SetVisible(false);
		return;
	}

	highScoreValue->SetText(to_string(highScore));
	highScoreText->SetVisible(true);
	highScoreValue->SetVisible(true);
}

void LevelsMenu::OnLevelButtonExitHover()
{
	highScoreText->SetVisible(false);
	highScoreValue->SetVisible(false);
}

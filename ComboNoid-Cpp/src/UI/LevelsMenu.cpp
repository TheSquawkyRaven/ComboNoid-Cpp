#include "LevelsMenu.h"
#include "../Game.h"
#include "MenuManager.h"

#include <cmath>


LevelsMenu::LevelsMenu(MenuManager* menuManager, Game* game) : Node(game), menuManager(menuManager)
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
	AddChild(backButton);
	AddChild(highScoreText);
	AddChild(highScoreValue);

	Vector2 hPos = highScoreTextPos;
	highScoreText->Init();
	highScoreText->pos = hPos;
	highScoreText->SetText("High Score:", fontSize, highScoreTextColor);

	hPos.y += highScoreValueYOffset;
	highScoreValue->Init();
	highScoreValue->pos = hPos;
	highScoreValue->SetText(" ", fontSize, highScoreTextColor);

	highScoreText->SetVisible(false);
	highScoreValue->SetVisible(false);

	backButton->Init(4);
	backButton->pos = backButtonPos;
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
		AddChild(button);
		button->Init(1);
		button->pos = p;
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

void LevelsMenu::OnBackButtonPressed()
{
	menuManager->OpenMainMenu();
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

void LevelsMenu::SetVisible(bool visible)
{
	Node::SetVisible(visible);
	highScoreText->SetVisible(false);
	highScoreValue->SetVisible(false);
}

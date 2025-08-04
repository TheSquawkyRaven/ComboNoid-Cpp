#include "LevelsMenu.h"
#include "../Game.h"
#include "MenuManager.h"

#include <cmath>


LevelsMenu::LevelsMenu(MenuManager* menuManager, Game* game) : game(game), menuManager(menuManager)
{
	backButton = new Button(game);

	backButtonPos.x = game->renderX / 2.0f;
	backButtonPos.y = backButtonY;
}

void LevelsMenu::Init()
{
	backButton->Init(backButtonPos, 4);
	backButton->InitText("Back", fontSize, textColor);
	backButton->pressed = [this]()
	{
		this->OnBackButtonPressed();
	};

	int x = 0;
	int y = 0;
	Vector2 p;
	int c = 1;
	for (auto& levelName : levels)
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
		button->InitText(to_string(c), fontSize, textColor);
		button->pressed = [this, levelName]()
		{
			this->OnLevelPressed(levelName);
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
}

void LevelsMenu::OnLevelPressed(const string& levelName)
{
	game->TriggerLoadLevel(levelName);
}

void LevelsMenu::LaunchFirstLevel()
{
	game->TriggerLoadLevel(levels.at(0));
}

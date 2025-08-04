#include "MainMenu.h"
#include "../Game.h"
#include "MenuManager.h"

#include <cmath>


MainMenu::MainMenu(MenuManager* menuManager, Game* game) : game(game), menuManager(menuManager)
{
	title = new Text(game);
	playButton = new Button(game);
	levelsButton = new Button(game);
	quitButton = new Button(game);

	buttons.push_back(playButton);
	buttons.push_back(levelsButton);
	buttons.push_back(quitButton);

	centerPos.x = game->renderX / 2.0f;
	centerPos.y = game->renderY / 2.0f;
}

void MainMenu::Init()
{
	Vector2 titlePos = Vector2(centerPos.x, centerPos.y + titleYOffset);
	title->Init(titlePos);
	title->SetText("Combonoid");
	title->SetFontSize(titleFontSize);
	title->SetColor(titleColor);
	title->Render();

	playButton->Init(centerPos, 4);
	playButton->InitText("Start", fontSize, textColor);
	playButton->pressed = [this]()
	{
		this->OnPlayButtonPressed();
	};

	levelsButton->Init(centerPos, 4);
	levelsButton->InitText("Levels", fontSize, textColor);
	levelsButton->pressed = [this]()
	{
		this->OnLevelsButtonPressed();
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

void MainMenu::Destroy(Game* game)
{
	title->Destroy(game);
	playButton->Destroy(game);
	levelsButton->Destroy(game);
	quitButton->Destroy(game);

	IDestroyable::Destroy(game);
}

void MainMenu::OnPlayButtonPressed()
{
	menuManager->LoadFirstLevel();
}

void MainMenu::OnLevelsButtonPressed()
{
	menuManager->OpenLevelsMenu();
}

void MainMenu::OnQuitButtonPressed()
{
	game->TriggerQuit();
}

void MainMenu::SetVisible(bool visible)
{
	title->SetVisible(visible);
	for (Button* button : buttons)
	{
		button->SetVisible(visible);
	}
}

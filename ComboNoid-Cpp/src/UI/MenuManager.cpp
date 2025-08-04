#include "MenuManager.h"
#include "../Game.h"

#include <cmath>


MenuManager::MenuManager(Game* game) : game(game)
{
	background = new Background(game);
}

void MenuManager::Init()
{
	background->Init();

	mainMenu = new MainMenu(this, game);
	levelsMenu = new LevelsMenu(this, game);

	mainMenu->Init();
	levelsMenu->Init();

	OpenMainMenu();
}

void MenuManager::Destroy(Game* game)
{
	background->Destroy(game);

	if (mainMenu != nullptr)
	{
		mainMenu->Destroy(game);
	}
	if (levelsMenu != nullptr)
	{
		levelsMenu->Destroy(game);
	}

	IDestroyable::Destroy(game);
}

void MenuManager::OpenMainMenu()
{
	mainMenu->SetVisible(true);
	levelsMenu->SetVisible(false);
}

void MenuManager::OpenLevelsMenu()
{
	mainMenu->SetVisible(false);
	levelsMenu->SetVisible(true);
}

void MenuManager::LoadFirstLevel()
{
	levelsMenu->LaunchFirstLevel();
}

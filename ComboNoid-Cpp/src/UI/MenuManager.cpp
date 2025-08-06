#include "MenuManager.h"
#include "../Game.h"

#include <cmath>


MenuManager::MenuManager(Game* game) : Node(game)
{
	background = new Background(game);
}

void MenuManager::Init()
{
	AddChild(background);
	background->Init();

	mainMenu = new MainMenu(this, game);
	levelsMenu = new LevelsMenu(this, game);

	AddChild(mainMenu);
	mainMenu->Init();
	AddChild(levelsMenu);
	levelsMenu->Init();

	OpenMainMenu();

	backgroundMusic = game->audioManager->LoadMusic("./assets/audio/menu_music.wav");
	game->audioManager->PlayMusic(backgroundMusic);
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

#include "MainMenu.h"
#include "../Game.h"

#include <cmath>


MainMenu::MainMenu(Game* game) : game(game)
{
	background = new Background(game);
	playButton = new Button(game);

	centerPos.x = game->renderX / 2.0f;
	centerPos.y = game->renderY / 2.0f;
}

void MainMenu::Init()
{
	background->Init();
	playButton->Init(centerPos);
}

void MainMenu::Destroy(Game* game)
{
	IDestroyable::Destroy(game);
	background->Destroy(game);
	playButton->Destroy(game);
}

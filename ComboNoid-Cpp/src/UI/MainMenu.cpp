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

	mainButtons.push_back(playButton);
	mainButtons.push_back(levelsButton);
	mainButtons.push_back(quitButton);

	fullScreenButton = new Button(game);
	musicButton = new Button(game);
	soundButton = new Button(game);

	optionButtons.push_back(fullScreenButton);
	optionButtons.push_back(musicButton);
	optionButtons.push_back(soundButton);

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

	playButton->Init(centerPos, 4);
	playButton->SetText("Start", fontSize, textColor);
	playButton->pressed = [this]()
	{
		this->OnPlayButtonPressed();
	};

	levelsButton->Init(centerPos, 4);
	levelsButton->SetText("Levels", fontSize, textColor);
	levelsButton->pressed = [this]()
	{
		this->OnLevelsButtonPressed();
	};

	quitButton->Init(centerPos, 4);
	quitButton->SetText("Quit", fontSize, textColor);
	quitButton->pressed = [this]()
	{
		this->OnQuitButtonPressed();
	};

	Vector2 p = Vector2(centerPos.x + centerOffset.x, centerPos.y + centerOffset.y);
	for (Button* button : mainButtons)
	{
		button->SetPos(p);
		p.y += ySpacing;
	}

	fullScreenButton->Init(centerPos, 4);
	fullScreenButton->SetText("Full OF", optionsFontSize, textColor);
	fullScreenButton->pressed = [this]()
	{
		this->OnFullScreenButtonPressed();
	};

	musicButton->Init(centerPos, 4);
	musicButton->SetText("Music ON", optionsFontSize, textColor);
	musicButton->pressed = [this]()
	{
		this->OnMusicButtonPressed();
	};

	soundButton->Init(centerPos, 4);
	soundButton->SetText("Sound ON", optionsFontSize, textColor);
	soundButton->pressed = [this]()
	{
		this->OnSoundButtonPressed();
	};

	p = Vector2(centerPos.x + optionsOffset.x, centerPos.y + optionsOffset.y);
	for (Button* button : optionButtons)
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
	
	fullScreenButton->Destroy(game);
	musicButton->Destroy(game);
	soundButton->Destroy(game);

	mainButtons.clear();
	optionButtons.clear();

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

void MainMenu::OnFullScreenButtonPressed()
{
	game->renderer->ToggleFullScreen();
	if (game->renderer->GetIsFullScreen())
	{
		fullScreenButton->SetText("Full ON", optionsFontSize, textColor);
	}
	else
	{
		fullScreenButton->SetText("Full OF", optionsFontSize, textColor);
	}
}

void MainMenu::OnMusicButtonPressed()
{
	game->audioManager->ToggleMusicMute();
	if (game->audioManager->GetMusicMuted())
	{
		musicButton->SetText("Music OF", optionsFontSize, textColor);
	}
	else
	{
		musicButton->SetText("Music ON", optionsFontSize, textColor);
	}
}

void MainMenu::OnSoundButtonPressed()
{
	game->audioManager->ToggleMixMute();
	if (game->audioManager->GetMixMuted())
	{
		soundButton->SetText("Sound OF", optionsFontSize, textColor);
	}
	else
	{
		soundButton->SetText("Sound ON", optionsFontSize, textColor);
	}
}

void MainMenu::SetVisible(bool visible)
{
	title->SetVisible(visible);
	for (Button* button : mainButtons)
	{
		button->SetVisible(visible);
	}
	for (Button* button : optionButtons)
	{
		button->SetVisible(visible);
	}
}

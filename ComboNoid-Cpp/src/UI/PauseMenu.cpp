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
	resumeButton->Destroy(game);
	quitButton->Destroy(game);

	IDestroyable::Destroy(game);
}

void PauseMenu::OnDestroy()
{
	IDrawable::Unregister(game);
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

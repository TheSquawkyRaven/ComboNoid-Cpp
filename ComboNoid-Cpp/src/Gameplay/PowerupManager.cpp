#include "PowerupManager.h"
#include "../Game.h"
#include "Gameplay.h"


PowerupManager::PowerupManager(Game* game, Gameplay* gameplay) : Node(game), gameplay(gameplay)
{
	powerupGainedClip = make_unique<Clip>(game->audioManager, "./assets/audio/powerup_gain.wav");
	powerupCreatedClip = make_unique<Clip>(game->audioManager, "./assets/audio/powerup_create.wav");
}

Powerup* PowerupManager::CreatePowerup(Powerup::Type type, Vector2& pos)
{
	powerupCreatedClip->Play();

	Powerup* powerup = new Powerup(game, gameplay);
	AddChild(powerup);

	powerup->gained = [this](Powerup* p)
	{
		this->OnPowerupGained(p);
	};
	powerup->fellOff = [this](Powerup* p)
	{
		this->OnPowerupFellOff(p);
	};
	powerup->Init(type, pos);

	powerups.insert(powerup);

	return powerup;
}

void PowerupManager::OnPowerupGained(Powerup* powerup)
{
	powerupGainedClip->Play();
	powerups.erase(powerup);
	powerup->Destroy(this);
}

void PowerupManager::OnPowerupFellOff(Powerup* powerup)
{
	powerups.erase(powerup);
	powerup->Destroy(this);
}

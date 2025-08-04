#include "PowerupManager.h"
#include "../Game.h"
#include "Gameplay.h"


PowerupManager::PowerupManager(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{

}

void PowerupManager::Init()
{

}

void PowerupManager::Destroy(Game* game)
{
	for (auto& powerup : powerups)
	{
		powerup->Destroy(game);
	}
	powerups.clear();

	IDestroyable::Destroy(game);
}

Powerup* PowerupManager::CreatePowerup(Powerup::Type type, Vector2& pos)
{
	Powerup* powerup = new Powerup(game, gameplay);
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
	powerups.erase(powerup);
	powerup->Destroy(game);
}

void PowerupManager::OnPowerupFellOff(Powerup* powerup)
{
	powerups.erase(powerup);
	powerup->Destroy(game);
}

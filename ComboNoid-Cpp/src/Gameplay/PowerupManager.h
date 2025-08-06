#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <set>
#include <memory>

#include "../Node/Node.h"
#include "Powerup.h"
#include "../Clip.h"

using namespace std;

class Game;
class Gameplay;

class PowerupManager : public Node
{
private:
	Gameplay* gameplay;

	unique_ptr<Clip> powerupGainedClip;
	unique_ptr<Clip> powerupCreatedClip;

	// Blocks in play
	set<Powerup*> powerups;

private:
	void OnPowerupGained(Powerup* powerup);
	void OnPowerupFellOff(Powerup* powerup);

public:
	PowerupManager(Game* game, Gameplay* gameplay);

	Powerup* CreatePowerup(Powerup::Type type, Vector2& pos);

};


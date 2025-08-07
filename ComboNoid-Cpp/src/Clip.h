#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <memory>

#include "AudioManager.h"

using namespace std;

class Game;

class Clip
{
private:
	shared_ptr<AudioManager> audioManager = nullptr;
	shared_ptr<Mix_Chunk> mix;

public:
	Clip(shared_ptr<AudioManager> audioManager, const string& path);

	void Play() const;

};

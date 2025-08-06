#include "Clip.h"
#include "Game.h"

Clip::Clip(shared_ptr<AudioManager> audioManager, const string& path)
{
	this->audioManager = audioManager;
	mix = audioManager->LoadMix(path);
}

void Clip::Play() const
{
	audioManager->PlayMix(mix, -1);
}

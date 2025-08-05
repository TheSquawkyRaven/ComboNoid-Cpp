#include "Clip.h"

Clip::Clip(AudioManager* audioManager, const string& path)
{
	this->audioManager = audioManager;
	mix = audioManager->LoadMix(path);
}

void Clip::Play() const
{
	audioManager->PlayMix(mix, -1);
}

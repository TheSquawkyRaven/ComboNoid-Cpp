#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <map>
#include <memory>

#include "Vector2.h"

using namespace std;

class AudioManager
{
private:
	inline static const int channels = 16;

	map<string, weak_ptr<Mix_Chunk>> mixMap;
	map<string, weak_ptr<Mix_Music>> musicMap;

public:
	inline static const float defaultMusicVolume = 0.175f;
	inline static const float defaultMixVolume = 1.0f;

public:
	AudioManager();

	shared_ptr<Mix_Music> LoadMusic(const string& path);
	shared_ptr<Mix_Chunk> LoadMix(const string& path);

	void PlayMusic(shared_ptr<Mix_Music> music);
	void PauseMusic(bool paused);
	void StopMusic();

	void PlayMix(shared_ptr<Mix_Chunk> mix, int channel = -1);
	
	void SetMixVolume(float volume) const;
	void SetMusicVolume(float volume) const;

};

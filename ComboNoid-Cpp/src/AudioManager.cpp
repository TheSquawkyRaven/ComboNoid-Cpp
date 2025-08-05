#include "AudioManager.h"

AudioManager::AudioManager()
{
    Mix_AllocateChannels(channels);
	SetMusicVolume(defaultMusicVolume);
	SetMixVolume(defaultMixVolume);
}

shared_ptr<Mix_Music> AudioManager::LoadMusic(const string& path)
{
    if (musicMap.contains(path))
    {
        shared_ptr<Mix_Music> loadedMusic = musicMap[path].lock();
        if (loadedMusic)
        {
            return loadedMusic;
        }
        musicMap.erase(path);
    }
    Mix_Music* music_ = Mix_LoadMUS(path.c_str());
    if (music_ == NULL)
    {
        printf("Failed to load music audio, path: %s! SDL_MIX_ERROR: %s\n", path.c_str(), Mix_GetError());
        return nullptr;
    }

    // Destruction uses Mix_FreeMusic
    shared_ptr<Mix_Music> music = shared_ptr<Mix_Music>(music_, Mix_FreeMusic);
    musicMap[path] = music;

    return music;
}

shared_ptr<Mix_Chunk> AudioManager::LoadMix(const string& path)
{
    if (mixMap.contains(path))
    {
        shared_ptr<Mix_Chunk> loadedMix = mixMap[path].lock();
        if (loadedMix)
        {
            return loadedMix;
        }
        mixMap.erase(path);
    }
    Mix_Chunk* mix_ = Mix_LoadWAV(path.c_str());
    if (mix_ == NULL)
    {
        printf("Failed to load mix chunk audio, path: %s! SDL_MIX_ERROR: %s\n", path.c_str(), Mix_GetError());
        return nullptr;
    }

    // Destruction uses Mix_FreeChunk
    shared_ptr<Mix_Chunk> mix = shared_ptr<Mix_Chunk>(mix_, Mix_FreeChunk);
    mixMap[path] = mix;

	return mix;
}

void AudioManager::PlayMusic(shared_ptr<Mix_Music> music)
{
    Mix_PlayMusic(music.get(), -1);
}

void AudioManager::PauseMusic(bool paused)
{
    if (paused)
    {
        Mix_PauseMusic();
    }
    else
    {
        Mix_ResumeMusic();
	}
}

void AudioManager::StopMusic()
{
    Mix_HaltMusic();
}

void AudioManager::PlayMix(shared_ptr<Mix_Chunk> mix, int channel)
{
    Mix_PlayChannel(channel, mix.get(), 0);
}

void AudioManager::SetMixVolume(float volume) const
{
	int v = static_cast<int>(volume * MIX_MAX_VOLUME);
    for (int i = 0; i < channels; i++)
    {
        Mix_Volume(i, v);
	}
}

void AudioManager::SetMusicVolume(float volume) const
{
    int v = static_cast<int>(volume * MIX_MAX_VOLUME);
	Mix_VolumeMusic(v);
}

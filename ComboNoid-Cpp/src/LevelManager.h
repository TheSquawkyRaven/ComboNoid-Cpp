#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Level;
class Layer;

class LevelManager
{
public:

	LevelManager();
	void Init();

	shared_ptr<Level> LoadLevel(const string& levelName);

};

class Level
{
public:
	enum TileType
	{
		EMPTY = 0,
		RED = 1,
		GREEN = 12,
		BLUE = 23,
		PURPLE = 34,
		YELLOW = 45,
		ORANGE = 56,
		WOOD = 67,
		STONE = 78,
	};

	string path;
	XMLDocument doc;

	Layer* tiles = nullptr;

	Level(const string& path);

	void Load();
};

class Layer
{
public:
	int width = 0;
	int height = 0;

	vector<int> data;
};

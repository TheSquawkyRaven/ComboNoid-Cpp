#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <map>
#include <vector>

#include "../Node/Node.h"
#include "../UI/Text.h"

class Game;

class Tutorial : public Node
{
private:
	inline static const int drawLayer = -10;
	inline static const int fontSize = 16;
	inline static const SDL_Color textColor{ 255, 255, 255, 180 };

	inline static map<int, vector<pair<string, Vector2>>> textsMap = {
		{ 0, {{
			"Move paddle", Vector2(100, 150)
			},{
			"<left/right", Vector2(100, 170)
			},{
			"arrow keys>", Vector2(100, 184)
			},{
			"Launch ball", Vector2(380, 150)
			},{
			"<space>", Vector2(380, 170)
			}}
		},
		{ 1, {{
			"Parry ball", Vector2(120, 250)
			},{
			"increases combo", Vector2(120, 264)
			},{
			"<space>", Vector2(120, 284)
			},{
			"High combo:", Vector2(360, 250)
			},{
			"More damage", Vector2(360, 264)
			},{
			"More score", Vector2(360, 278)
			},{
			"x8 -> Break Wood", Vector2(360, 292)
			},{
			"x12 -> Break Stone", Vector2(360, 306)
			}}
		},
		{ 5, {{
			"Junimo", Vector2(80, 100)
			}}
		},
		{ 7, {{
			"Mario", Vector2(80, 100)
			}}
		},
		{ 8, {{
			"Some Logo", Vector2(80, 100)
			}}
		},
	};

	vector<Text*> texts;

private:
	void DestroyTexts();

public:
	Tutorial(Game* game);

	void LevelLoaded(int levelIndex);

};


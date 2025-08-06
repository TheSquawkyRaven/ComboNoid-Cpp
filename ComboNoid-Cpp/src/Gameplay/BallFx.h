#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>
#include <memory>
#include <vector>

#include "../Components.h"
#include "../Vector2.h"
#include "../Clip.h"

using namespace std;

class Game;
class Gameplay;

class BallFx : public IDestroyable, public ITransform, public IDrawable
{
private:
	inline static const vector<int> comboTier = {
		1,
		2,
		3,
		4,
		5,
		6,
		8,
		12,
	};
	inline static SDL_Rect wheelRect{ 0, 0, 16, 16 };
	inline static const float wheelRotateSpeed = 180.0f;

	Game* game;

	int wheelX = -1;

public:
	float timeFactor = 1.0f;

	BallFx(Game* game);
	void Init();

	void OnDestroy() override;
	void BallUpdate(Vector2& pos);

	void SetCombo(int combo);

};


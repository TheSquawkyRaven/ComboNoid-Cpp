#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <functional>

#include "../Node/NodeSprite.h"
#include "../Node/NodeRectCollider.h"

class Gameplay;

class Powerup : public NodeSprite, public NodeRectCollider
{
public:
	enum Type
	{
		ENLARGE,
		SHRINK,
		SPLIT_BALL,
		EXTRA_BALL,
		ENLARGE_BALL,
		SLOW_BALL,
	};

	static Type RandomType();

private:
	inline static SDL_Rect rectEnlarge{ 0, 0, 16, 16 };
	inline static SDL_Rect rectShrink{ 16, 0, 16, 16 };
	inline static SDL_Rect rectSplitBall{ 0, 16, 16, 16 };
	inline static SDL_Rect rectExtraBall{ 16, 16, 16, 16 };
	inline static SDL_Rect rectEnlargeBall{ 0, 32, 16, 16 };
	inline static SDL_Rect rectSlowBall{ 16, 32, 16, 16 };

	inline static const Vector2 rectSize{ 16, 9 };

	Gameplay* gameplay;

	Type type = ENLARGE;
	float speed = 100;

public:
	inline static const float spawnChance = 0.1f;

	function<void(Powerup*)> gained;
	function<void(Powerup*)> fellOff;

public:

	Powerup(Game* game, Gameplay* gameplay);
	void Init(Type type, Vector2 pos);

	inline void AddedAsChild(Node* parent) override { NodeRectCollider::AddedAsChild(parent); }
	inline void RemovedAsChild(Node* parent) override { NodeRectCollider::RemovedAsChild(parent); }
	inline void Draw() override { NodeSprite::Draw(); }

	void Update() override;
	void OnCollision(NodeRectCollider* rect, Tree::Layer layer) override;

	void SetType(Type type);

};

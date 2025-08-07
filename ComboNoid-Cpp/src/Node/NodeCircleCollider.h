#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "Node.h"
#include "Tree.h"

using namespace std;

class NodeRectCollider;

class NodeCircleCollider : public virtual Node
{
public:
	/// <summary>
	/// Radius of the 
	/// </summary>
	float radius = 0.0f;

public:
	NodeCircleCollider(Game* game);

	void AddedAsChild(Node* parent) override;
	void RemovedAsChild(Node* parent) override;

	/// <summary>
	/// Returns the scaled radius
	/// </summary>
	float GetActualRadius() const;

	/// <summary>
	/// Circle to Rect collision check with NodeRectCollider
	/// </summary>
	/// <param name="rect">Other rect</param>
	/// <returns>If overlaps</returns>
	bool CheckCollision(NodeRectCollider* rect) const;

	inline virtual void OnCollision(NodeRectCollider* rect, Tree::Layer layer) {};

};

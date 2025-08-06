#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>

#include "Node.h"
#include "Tree.h"

using namespace std;

class NodeRectCollider : public virtual Node
{
public:
	/// <summary>
	/// Size of the rect
	/// </summary>
	Vector2 size{ 0, 0 };

	Tree::Layer layer = Tree::BLOCK;

public:
	NodeRectCollider(Game* game);

	void AddedAsChild(Node* parent) override;
	void RemovedAsChild(Node* parent) override;
	
	SDL_Rect GetDestRect() const;

	/// <summary>
	/// Returns the centered position
	/// </summary>
	Vector2 GetActualPos() const;
	/// <summary>
	/// Returns the scaled size
	/// </summary>
	Vector2 GetActualSize() const;

	/// <summary>
	/// AABB collision check with another NodeRectCollider
	/// </summary>
	/// <param name="rect">Other rect</param>
	/// <returns>If overlaps</returns>
	bool CheckCollision(NodeRectCollider* rect) const;

	inline virtual void OnCollision(NodeRectCollider* rect, Tree::Layer layer) {};

};

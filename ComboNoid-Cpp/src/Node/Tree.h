#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>
#include <map>
#include <memory>
#include <queue>

using namespace std;

class Game;
class Node;
class NodeRectCollider;
class NodeCircleCollider;

/// <summary>
/// Manages Nodes
/// </summary>
class Tree
{
public:
	enum Layer
	{
		BALL = 0,
		PADDLE = 1,
		BLOCK = 2,
		WALL = 3,
		POWERUP = 4
	};
private:
	inline static const map<Layer, vector<Layer>> collisionDefinition = {
		{ BALL, { PADDLE, BLOCK, WALL } }, // Ball - Collide with Paddle, Block and Wall
		{ PADDLE, {} }, // Paddle
		{ BLOCK, {} }, // Block
		{ WALL, {} }, // Wall
		{ POWERUP, { PADDLE }} // Powerup - Collide with Paddle
	};

	Node* root = nullptr;

	vector<NodeCircleCollider*> circleColliders{};
	map<int, vector<NodeRectCollider*>> rectColliders{
		{ PADDLE, {} },
		{ BLOCK, {} },
		{ WALL, {} },
		{ POWERUP, {} },
	};

	queue<Node*> destructionQueue;

public:
	Game* game;

private:
	void RunDestructions();
	void RunCollisions();

	void RunDraw();

public:
	Tree(Game* game);

	inline Node* GetRoot() const { return root; }
	void SetRoot(Node* rootNode);

	/// <summary>
	/// Executes the lifecycle of the tree, from the root
	/// </summary>
	void Run();

	/// <summary>
	/// Executs the input lifecycle of the tree, from the root
	/// </summary>
	void RunInput(SDL_Event& event);

	/// <summary>
	/// Registers a rectangular collider with a specified layer. Can NOT be BALL.
	/// </summary>
	void RegisterCollider(NodeRectCollider* collider, Layer layer);
	void UnregisterCollider(NodeRectCollider* collider, Layer layer);
	/// <summary>
	/// Layer is always BALL as per collision definition.
	/// </summary>
	/// <param name="collider"></param>
	void RegisterCollider(NodeCircleCollider* collider);
	void UnregisterCollider(NodeCircleCollider* collider);

	/// <summary>
	/// Queues a node for destruction
	/// </summary>
	void _QueueDestruction(Node* node);

};

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <vector>

#include "../Vector2.h"

using namespace std;

class Game;

class Node
{
private:
	vector<Node*> children;

	bool visible = true;

public:
	Game* game;
	
	Vector2 pos{ 0, 0 };
	Vector2 scale{ 1, 1 };
	float rot = 0;

public:
	inline const vector<Node*>& GetChildren() const { return children; }
	inline bool GetVisible() const { return visible; }
	inline virtual void SetVisible(bool visible) { this->visible = visible; }

	Node(Game* game);

	void _Input(SDL_Event& event);
	void _Update();
	void _Draw();
	void Destroy(Node* parent);

	inline virtual void AddedAsChild(Node* parent) {};
	inline virtual void RemovedAsChild(Node* parent) {};
	inline virtual void Input(SDL_Event& event) {};
	inline virtual void Update() {};
	inline virtual void Draw() {};
	inline virtual void OnDestroy() {};

	void AddChild(Node* child);
	void RemoveChild(Node* child);

};

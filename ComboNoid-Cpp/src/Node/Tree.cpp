#include "Tree.h"
#include "../Game.h"
#include "Node.h"
#include "NodeRectCollider.h"
#include "NodeCircleCollider.h"

Tree::Tree(Game* game) : game(game)
{

}

void Tree::SetRoot(Node* rootNode)
{
	if (rootNode == nullptr)
	{
		printf("Tree: SetRoot called with null rootNode!\n");
		return;
	}
	root = rootNode;
}

void Tree::Run()
{
	RunDestructions();
	root->_Update();
	RunDestructions();
	RunDraw();
}

void Tree::RunDraw()
{
	game->renderer->Clear();
	root->_Draw();
	game->renderer->Flush();
}

void Tree::RunInput(SDL_Event& event)
{
	root->_Input(event);
}

void Tree::RunDestructions()
{
	while (!destructionQueue.empty())
	{
		Node* obj = destructionQueue.front();
		destructionQueue.pop();

		obj->OnDestroy();
		delete obj;
	}
}

void Tree::RunCollisions()
{
	for (auto& pair : collisionDefinition)
	{
		Layer from = pair.first;
		const vector<Layer>& toLayers = pair.second;

		if (from == BALL)
		{
			const vector<NodeCircleCollider*>& fromColliders = circleColliders;
			for (auto& to : toLayers)
			{
				// Skip handling if to is BALL
				const vector<NodeRectCollider*>& toColliders = rectColliders.at(to);
				for (NodeCircleCollider* fromCol : fromColliders)
				{
					for (NodeRectCollider* toCol : toColliders)
					{
						if (fromCol->CheckCollision(toCol))
						{
							fromCol->OnCollision(toCol, to);
						}
					}
				}
			}
			continue;
		}

		const vector<NodeRectCollider*>& fromColliders = rectColliders.at(from);
		for (auto& to : toLayers)
		{
			for (NodeRectCollider* fromCol : fromColliders)
			{
				const vector<NodeRectCollider*>& toColliders = rectColliders.at(to);
				for (NodeRectCollider* toCol : toColliders)
				{
					if (fromCol->CheckCollision(toCol))
					{
						fromCol->OnCollision(toCol, to);
					}
				}
			}
			// Skip handling if to is BALL
			continue;
		}
	}
}

void Tree::RegisterCollider(NodeRectCollider* collider, Layer layer)
{
	rectColliders.at(layer).push_back(collider);
}

void Tree::RegisterCollider(NodeCircleCollider* collider)
{
	circleColliders.push_back(collider);
}

void Tree::UnregisterCollider(NodeRectCollider* collider, Layer layer)
{
	auto& colliders = rectColliders.at(layer);
	auto it = find(colliders.begin(), colliders.end(), collider);
	if (it != colliders.end())
	{
		colliders.erase(it);
	}
}

void Tree::UnregisterCollider(NodeCircleCollider* collider)
{
	auto it = find(circleColliders.begin(), circleColliders.end(), collider);
	if (it != circleColliders.end())
	{
		circleColliders.erase(it);
	}
}

void Tree::_QueueDestruction(Node* node)
{
	destructionQueue.push(node);
}

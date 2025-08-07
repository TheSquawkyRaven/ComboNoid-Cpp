#include "Node.h"
#include "../Game.h"

Node::Node(Game* game) : game(game)
{

}

void Node::SetVisible(bool visible)
{
	this->visible = visible;
	for (int i = 0; i < children.size(); i++)
	{
		children.at(i)->SetVisible(visible);
	}
}

void Node::_Input(SDL_Event& event)
{
	Input(event);
	for (int i = 0; i < children.size(); i++)
	{
		children.at(i)->_Input(event);
	}
}

void Node::_Update()
{
	Update();
	for (int i = 0; i < children.size(); i++)
	{
		children.at(i)->_Update();
	}
}

void Node::_Draw()
{
	if (!visible)
	{
		return;
	}

	Draw();
	for (int i = 0; i < children.size(); i++)
	{
		children.at(i)->_Draw();
	}
}

void Node::Destroy(Node* parent)
{
	if (markedForDestruction)
	{
		return;
	}
	markedForDestruction = true;

	if (parent)
	{
		parent->RemoveChild(this);
	}
	for (int i = children.size() - 1; i >= 0; i--)
	{
		children.at(i)->Destroy(this);
	}
	game->tree->_QueueDestruction(this);
}

void Node::AddChild(Node* child)
{
	children.push_back(child);
	child->AddedAsChild(this);
}

void Node::RemoveChild(Node* child)
{
	auto it = find(children.begin(), children.end(), child);
	if (it != children.end())
	{
		children.erase(it);
		child->RemovedAsChild(this);
	}
	else
	{
		printf("Node: Tried to remove a child that is not in the list!\n");
	}
}

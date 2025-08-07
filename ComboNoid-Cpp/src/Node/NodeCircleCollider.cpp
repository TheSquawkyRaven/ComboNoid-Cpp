#include "NodeCircleCollider.h"
#include "../Game.h"
#include "NodeRectCollider.h"

NodeCircleCollider::NodeCircleCollider(Game* game) : Node(game)
{

}

void NodeCircleCollider::AddedAsChild(Node* parent)
{
	game->tree->RegisterCollider(this);
}

void NodeCircleCollider::RemovedAsChild(Node* parent)
{
	game->tree->UnregisterCollider(this);
}

float NodeCircleCollider::GetActualRadius() const
{
	return radius * scale.x;
}

bool NodeCircleCollider::CheckCollision(NodeRectCollider* rect) const
{
	SDL_Rect to = rect->GetDestRect();
	float closestX = clamp(pos.x, static_cast<float>(to.x), static_cast<float>(to.x + to.w));
	float closestY = clamp(pos.y, static_cast<float>(to.y), static_cast<float>(to.y + to.h));

	float dx = pos.x - closestX;
	float dy = pos.y - closestY;

	float radius = GetActualRadius();
	return (dx * dx + dy * dy) < (radius * radius);
}

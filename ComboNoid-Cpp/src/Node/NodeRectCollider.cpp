#include "NodeRectCollider.h"
#include "../Game.h"

NodeRectCollider::NodeRectCollider(Game* game) : Node(game)
{

}

void NodeRectCollider::AddedAsChild(Node* parent)
{
	game->tree->RegisterCollider(this, layer);
}

void NodeRectCollider::RemovedAsChild(Node* parent)
{
	game->tree->UnregisterCollider(this, layer);
}

SDL_Rect NodeRectCollider::GetDestRect() const
{
	SDL_Rect rect{};
	float w = size.x * scale.x;
	float h = size.y * scale.y;
	rect.x = pos.x - w / 2.0f;
	rect.y = pos.y - h / 2.0f;
	rect.w = static_cast<int>(w);
	rect.h = static_cast<int>(h);

	return move(rect);
}

bool NodeRectCollider::CheckCollision(NodeRectCollider* rect) const
{
	SDL_Rect from = GetDestRect();
	SDL_Rect to = rect->GetDestRect();

	// AABB
	return (from.x < to.x + to.w &&
		from.x + from.w > to.x &&
		from.y < to.y + to.h &&
		from.y + from.h > to.y);
}

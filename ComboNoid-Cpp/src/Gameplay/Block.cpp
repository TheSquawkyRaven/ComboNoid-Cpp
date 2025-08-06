#include "Block.h"
#include "../Game.h"
#include "Gameplay.h"

Block::Block(Game* game, Gameplay* gameplay) : NodeSprite(game), NodeRectCollider(game), Node(game), gameplay(gameplay)
{
	layer = Tree::BLOCK;
}

void Block::Init(Color color, Vector2& pos)
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/block.png");
	SetTexture(texture);

	SetColor(color);

	this->pos = pos;
	centered = true;

	cropRect.w = blockSize.x;
	cropRect.h = blockSize.y;
}

void Block::Update()
{
	if (!breaking)
	{
		return;
	}

	breakingTimer += game->GetDeltaTime();

	int frame = Game::AnimateFrame(breakingFrames, breakingTimer, breakingTime / breakingFrames, false);
	if (frame == -1)
	{
		destroyed(this);
		return;
	}
	cropRect.x = (breakingStartFrame + frame) * blockSize.x;
	cropRect.y = color * blockSize.y;
}

void Block::SetColor(Color color)
{
	this->color = color;
	size = blockSize;
	cropRect.x = 0;
	cropRect.y = color * blockSize.y;

	if (color == WOOD)
	{
		isStrong = true;
		hp = damageToBreakWood;
	}
	else if (color == STONE)
	{
		isStrong = true;
		hp = damageToBreakStone;
	}
	else
	{
		hp = color + 1; // RED = 1hp
	}
}

bool Block::DamageBlock(int damage)
{
	if (isStrong && damage < hp)
	{
		return false;
	}

	hp -= damage;
	if (hp <= 0)
	{
		breaking = true;

		// The better the block the higher the chances of spawning a powerup
		if (Powerup::spawnChance * (color + 1) > game->RandomFloatRange(0.0f, 1.0f))
		{
			SpawnPowerup();
		}
		broken(this);
		return true;
	}
	hit(this);

	int frame = breakingStartFrame - hp;
	cropRect.x = frame * blockSize.x;
	cropRect.y = color * blockSize.y;

	return false;
}

void Block::SpawnPowerup()
{
	Vector2 p = pos + Vector2(8, 8);
	gameplay->powerupManager->CreatePowerup(Powerup::RandomType(), p);
}

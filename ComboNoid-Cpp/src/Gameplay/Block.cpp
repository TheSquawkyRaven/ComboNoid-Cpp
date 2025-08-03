#include "Block.h"
#include "../Game.h"
#include "Gameplay.h"

Block::Block(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Block::Init(Color color, Vector2& pos, bool spawnPowerup)
{
	IUpdatable::Register(game);
	IDrawable::Register(game);

	gameplay->RegisterBlock(this);

	this->spawnPowerup = spawnPowerup;

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/block.png");
	SetTexture(texture);

	SetColor(color);

	this->pos = pos;

	// Only need to place once
	PlaceTexture(this);
	PlaceCol(this);
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
		broken(this);
		return;
	}
	CropTexture((breakingStartFrame + frame) * blockSize.x, color * blockSize.y, blockSize.x, blockSize.y);
}

void Block::OnDestroy()
{
	IDrawable::Unregister(game);
	IUpdatable::Unregister(game);
}

void Block::SetColor(Color color)
{
	this->color = color;
	IRectCollidable::SetSize(blockSize.x, blockSize.y);
	CropTexture(0, color * blockSize.y, blockSize.x, blockSize.y);

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
		gameplay->UnregisterBlock(this);
		if (spawnPowerup)
		{
			SpawnPowerup();
		}
		return true;
	}

	int frame = breakingStartFrame - hp;
	CropTexture(frame * blockSize.x, color * blockSize.y, blockSize.x, blockSize.y);

	return false;
}

void Block::SpawnPowerup()
{
	Vector2 p = pos + Vector2(8, 8);
	gameplay->powerupManager->CreatePowerup(Powerup::RandomType(), p);
}

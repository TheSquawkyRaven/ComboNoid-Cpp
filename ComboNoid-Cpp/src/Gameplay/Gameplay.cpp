#include "Gameplay.h"
#include "../Game.h"


Gameplay::Gameplay(Game* game) : IUpdatable(game), game(game)
{
	paddle = new Paddle(game);
}

void Gameplay::Init()
{
	printf("Gameplay Initialized\n");
	paddle->Init();
}

void Gameplay::Update()
{

}

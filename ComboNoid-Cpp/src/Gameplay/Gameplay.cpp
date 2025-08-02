#include "Gameplay.h"
#include "../Game.h"
#include "Paddle.h"
#include "BallManager.h"
#include "BlockManager.h"
#include "Wall.h"


Gameplay::Gameplay(Game* game) : IUpdatable(game), game(game)
{
	paddle = new Paddle(game);
	ballManager = new BallManager(game);
	ballManager->doAttach = [this](Ball* ball)
	{
		this->OnBallManagerDoAttach(ball);
	};
	blockManager = new BlockManager(game);

	topWall = new Wall(game);
	leftWall = new Wall(game);
	rightWall = new Wall(game);
}

void Gameplay::Init()
{
	paddle->Init();
	ballManager->Init();
	blockManager->Init();

	topWall->Init(Wall::TOP);
	leftWall->Init(Wall::LEFT);
	rightWall->Init(Wall::RIGHT);

	printf("Gameplay Initialized\n");
}

void Gameplay::Update()
{

}

void Gameplay::OnBallManagerDoAttach(Ball* ball)
{
	paddle->AttachBall(ball);
}

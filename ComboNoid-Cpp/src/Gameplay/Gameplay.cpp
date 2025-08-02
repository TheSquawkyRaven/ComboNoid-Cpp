#include "Gameplay.h"
#include "../Game.h"
#include "BallManager.h"
#include "Paddle.h"
#include "Wall.h"


Gameplay::Gameplay(Game* game) : IUpdatable(game), game(game)
{
	paddle = new Paddle(game);
	ballManager = new BallManager(game);
	ballManager->doAttach = [this](Ball* ball)
	{
		this->OnBallManagerDoAttach(ball);
	};

	topWall = new Wall(game);
	leftWall = new Wall(game);
	rightWall = new Wall(game);
}

void Gameplay::Init()
{
	printf("Gameplay Initialized\n");
	ballManager->Init();
	paddle->Init();

	topWall->Init(Wall::TOP);
	leftWall->Init(Wall::LEFT);
	rightWall->Init(Wall::RIGHT);
}

void Gameplay::Update()
{

}

void Gameplay::OnBallManagerDoAttach(Ball* ball)
{
	paddle->AttachBall(ball);
}

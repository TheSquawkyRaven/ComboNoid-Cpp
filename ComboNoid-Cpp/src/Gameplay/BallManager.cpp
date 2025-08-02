#include "BallManager.h"
#include "../Game.h"


BallManager::BallManager(Game* game) : game(game)
{

}

void BallManager::Init()
{
	CreateBall(true);
}

void BallManager::CreateBall(bool fromStock)
{
	unique_ptr<Ball> ball = make_unique<Ball>(game);
	ball->destroyed = [this](Ball* b)
	{
		this->OnBallDestroyed(b);
	};
	ball->Init();

	if (fromStock)
	{
		ballsStock--;
		doAttach(ball.get());
	}

	balls.push_back(move(ball));
}

void BallManager::OnBallDestroyed(Ball* ball)
{
	// remove from balls vector (destrctor)
	balls.erase(remove_if(balls.begin(), balls.end(), [ball](const unique_ptr<Ball>& b)
	{
		return b.get() == ball;
	}),
	balls.end());

	if (balls.empty())
	{
		// Lost all balls in play
		if (ballsStock <= 0)
		{
			// Game over, no more balls in stock
			printf("Game Over! No more balls in stock.\n");
			return;
		}
		printf("Ball destroyed, creating a new one. Balls left in stock: %d\n", ballsStock);
		CreateBall(true);
	}
}

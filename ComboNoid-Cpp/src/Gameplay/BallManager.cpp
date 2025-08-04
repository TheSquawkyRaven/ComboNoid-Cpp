#include "BallManager.h"

#include <ctime>
#include <cstdlib>

#include "../Game.h"
#include "Gameplay.h"
#include "Combo.h"

BallManager::BallManager(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{

}

void BallManager::Init()
{
	IUpdatable::Register(game);
	CreateBall(true);
}

void BallManager::Destroy(Game* game)
{
	for (auto& ball : balls)
	{
		ball->Destroy(game);
	}
	balls.clear();

	IDestroyable::Destroy(game);
}

void BallManager::OnDestroy()
{
	IUpdatable::Unregister(game);
}

void BallManager::Update()
{
	UpdateTimer();
}

void BallManager::UpdateTimer()
{
	if (enlarge)
	{
		enlargeTimer -= game->GetDeltaTime();
		if (enlargeTimer < 0)
		{
			enlarge = false;
			EndEnlargeBall();
		}
	}
	if (slow)
	{
		slowTimer -= game->GetDeltaTime();
		if (slowTimer < 0)
		{
			slow = false;
			EndSlowBall();
		}
	}
}

Ball* BallManager::CreateBall(bool fromStock)
{
	Ball* ball = new Ball(game, gameplay);
	ball->fellOff = [this](Ball* b)
	{
		this->OnBallFellOff(b);
	};
	ball->Init();

	balls.insert(ball);

	if (fromStock)
	{
		ballsStock--;
		doAttach(ball);
	}
	
	return ball;
}

void BallManager::OnBallFellOff(Ball* ball)
{
	printf("Ball fell off at position (%.2f, %.2f)\n", ball->pos.x, ball->pos.y);
	// Remove from balls vector (destrctor)
	balls.erase(ball);

	// Ball fell off, destroy it
	ball->Destroy(game);

	if (balls.empty())
	{
		// Lost all balls in play
		gameplay->combo->BallLost();
		if (ballsStock <= 0)
		{
			// Game over, no more balls in stock
			printf("Game Over! No more balls in stock.\n");
			return;
		}
		printf("Ball fell off, creating a new one. Balls left in stock: %d\n", ballsStock);
		CreateBall(true);
	}
}

void BallManager::GainExtraBall()
{
	ballsStock++;
}

void BallManager::SplitBall()
{
	// Copy to prevent inserting while looping
	set<Ball*> originalBalls = balls;

	for (auto& ball : originalBalls)
	{
		Ball* ball1 = CreateBall(false);
		Ball* ball2 = CreateBall(false);

		ball1->pos = ball->pos;
		ball2->pos = ball->pos;

		Vector2 r = Vector2(Game::RandomFloatRange(0, 1), Game::RandomFloatRange(0, 1));
		r.Normalize();

		ball1->direction = r;

		r = Vector2(Game::RandomFloatRange(0, 1), Game::RandomFloatRange(0, 1));
		r.Normalize();
		ball2->direction = r;

		if (slow)
		{
			ball1->timeFactor = ballSlowFactor;
			ball2->timeFactor = ballSlowFactor;
		}
		if (enlarge)
		{
			ball1->SetSize(Ball::LARGE);
			ball2->SetSize(Ball::LARGE);
		}

		ball1->SetComboDamage(ball->GetComboDamage());
		ball2->SetComboDamage(ball->GetComboDamage());
	}
}

void BallManager::EnlargeBall()
{
	enlarge = true;
	enlargeTimer = enlargeTime;

	for (auto& ball : balls)
	{
		ball->SetSize(Ball::LARGE);
	}
}

void BallManager::SlowBall()
{
	slow = true;
	slowTimer = slowTime;

	for (auto& ball : balls)
	{
		ball->timeFactor = ballSlowFactor;
	}
}

void BallManager::EndEnlargeBall()
{
	for (auto& ball : balls)
	{
		ball->SetSize(Ball::NORMAL);
	}
}

void BallManager::EndSlowBall()
{
	for (auto& ball : balls)
	{
		ball->timeFactor = 1.0f;
	}
}

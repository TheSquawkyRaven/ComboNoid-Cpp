#include "BallManager.h"

#include <ctime>
#include <cstdlib>

#include "../Game.h"
#include "Gameplay.h"
#include "Combo.h"

BallManager::BallManager(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
	text = new Text(game);
	ballDroppedClip = make_unique<Clip>(game->audioManager, "./assets/audio/ball_drop.wav");
}

void BallManager::Init()
{
	ballCountPos.x = 40;
	ballCountPos.y = game->renderY - 12;

	text->Init(ballCountPos);
	text->SetFontSize(fontSize);
	text->SetColor(textColor);

	CreateBall(true);
}

void BallManager::Destroy(Game* game)
{
	text->Destroy(game);
	for (auto& ball : balls)
	{
		ball->Destroy(game);
	}
	balls.clear();

	IDestroyable::Destroy(game);
}

void BallManager::UpdateBallCount()
{
	text->SetText("Balls: " + to_string(ballsStock));
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
		UpdateBallCount();
		doAttach(ball);
	}
	
	return ball;
}

void BallManager::OnBallFellOff(Ball* ball)
{
	// Remove from balls vector (destrctor)
	balls.erase(ball);

	// Ball fell off, destroy it
	ball->Destroy(game);

	if (balls.empty())
	{
		ballDroppedClip->Play();
		// Lost all balls in play
		gameplay->combo->BallLost();
		if (ballsStock <= 0)
		{
			// Game over, no more balls in stock
			gameplay->GameOver(false);
			return;
		}
		CreateBall(true);
	}
}

void BallManager::GainExtraBall()
{
	ballsStock++;
	UpdateBallCount();
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

		Vector2 r = Vector2(Game::RandomFloatRange(-1, 1), Game::RandomFloatRange(-1, 1));
		r.Normalize();

		ball1->direction = r;

		r = Vector2(Game::RandomFloatRange(-1, 1), Game::RandomFloatRange(-1, 1));
		r.Normalize();
		ball2->direction = r;

		if (ball->IsBig())
		{
			float bigTimer = ball->GetBigTimer();
			ball1->Big(bigTimer);
			ball2->Big(bigTimer);
		}

		ball1->SetComboDamage(ball->GetComboDamage());
		ball2->SetComboDamage(ball->GetComboDamage());
	}
}

void BallManager::EnlargeBall()
{
	for (auto& ball : balls)
	{
		ball->Big();
	}
}

void BallManager::SlowBall()
{
	for (auto& ball : balls)
	{
		ball->Slow();
	}
}

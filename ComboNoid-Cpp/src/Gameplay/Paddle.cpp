#include "Paddle.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Ball.h"
#include "Powerup.h"
#include "Combo.h"

Paddle::Paddle(Game* game, Gameplay* gameplay) : NodeSprite(game), NodeRectCollider(game), Node(game), gameplay(gameplay)
{
	rLimit = game->renderX;
	paddleFlashClip = make_unique<Clip>(game->audioManager, "./assets/audio/paddle_flash.wav");
	layer = Tree::PADDLE;
}

void Paddle::Init()
{
	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/paddle.png");
	SetTexture(texture);

	SetSize(NORMAL);
}

void Paddle::Input(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
	{
		bool isDown = event.type == SDL_KEYDOWN;
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			leftInput = isDown;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT)
		{
			rightInput = isDown;
		}
		else if (event.key.keysym.sym == SDLK_SPACE)
		{
			spaceInput = isDown;
		}
	}
}

void Paddle::Update()
{
	UpdateTimer();
	UpdateFlash();

	int xInput = 0;
	xInput += leftInput ? -1 : 0;
	xInput += rightInput ? 1 : 0;

	pos.x += game->GetDeltaTime() * xInput * speed;

	float w = currentRect->w / 2.0f;
	if (pos.x - w < lLimit)
	{
		pos.x = lLimit + w;
	}
	else if (pos.x + w > rLimit)
	{
		pos.x = rLimit - w;
	}
	UpdateBall();
}

void Paddle::UpdateTimer()
{
	if (!sizeChanged)
	{
		return;
	}
	sizeChangedTimer -= game->GetDeltaTime();
	if (sizeChangedTimer < 0)
	{
		sizeChanged = false;
		SetSize(NORMAL);
	}
}

void Paddle::UpdateFlash()
{
	spaceJustPressed = spaceInput && !spaceWasHeld;
	spaceWasHeld = spaceInput;

	if (!pendingFlashBalls.empty())
	{
		// Handle pending flash balls
		// Only checks and removes the balls if they miss
		float currentTime = game->GetTotalTime();
		vector<Ball*> removingBalls;
		for (auto& pair : pendingFlashBalls)
		{
			Ball* ball = pair.first;
			float time = pair.second;

			if (currentTime - time > flashThreshold)
			{
				// Miss
				removingBalls.push_back(ball);
				ball->SetComboDamage(0);
				FlashMissBall();
			}
		}
		for (auto& ball : removingBalls)
		{
			pendingFlashBalls.erase(ball);
		}
	}

	if (!isFlashing && spaceJustPressed)
	{
		paddleFlashClip->Play();
		isFlashing = true;
		flashTimer = flashTime;

		if (!pendingFlashBalls.empty())
		{
			// Handle pending flash balls
			// Considered all as hits because the previous loop already checked for misses
			for (auto& pair : pendingFlashBalls)
			{
				Ball* ball = pair.first;
				FlashHitBall(ball);
			}

			pendingFlashBalls.clear();
		}
	}

	if (!isFlashing)
	{
		return;
	}
	flashTimer -= game->GetDeltaTime();
	if (flashTimer < 0)
	{
		if (!flashHit)
		{
			FlashMiss();
		}
		isFlashing = false;
		flashHit = false;
	}
}

void Paddle::SetSize(PaddleSize size)
{
	SDL_Rect* lastRect = currentRect;
	Vector2 rectSize;
	switch (size)
	{
		case SHORT:
			currentRect = &rectShort;
			rectSize = rectSizeShort;
			break;
		case NORMAL:
			currentRect = &rectNormal;
			rectSize = rectSizeNormal;
			break;
		case LONG:
			currentRect = &rectLong;
			rectSize = rectSizeLong;
			break;
		default:
			currentRect = &rectNormal;
			printf("Unknown Paddle Size of %d\n", size);
			break;
	}
	cropRect = *currentRect;

	// Collision size
	this->size.x = rectSize.x;
	this->size.y = rectSize.y;

	// Expand or contract from the center of the paddle
	int w = 0;
	int xOffset = 0;
	if (lastRect != nullptr)
	{
		// Expands/Contracts the paddle to the center
		w = (lastRect->w - currentRect->w) / 2;
	}
	else
	{
		xOffset = static_cast<int>(game->renderX / 2.0f);
		w = -currentRect->w / 2;
	}
	pos.x = pos.x + xOffset + w;
	pos.y = game->renderY - currentRect->h - 32;

	lastRect = currentRect;
}

void Paddle::AttachBall(Ball* ball)
{
	attachedBall = ball;
	ball->isAttached = true;
	UpdateBall();
}

void Paddle::UpdateBall()
{
	if (attachedBall == nullptr)
	{
		return;
	}

	attachedBall->pos.x = pos.x;
	attachedBall->pos.y = pos.y + rectOffset.y - attachedBall->radius;

	attachedBall->PostUpdate();

	if (spaceInput)
	{
		// Launch ball
		attachedBall->isAttached = false;
		// Add a tiny amount of random horizontal direction offset so the ball doesn't bounce up and down fully vertically forever
		attachedBall->direction = Vector2(game->RandomFloatRange(-0.01f, 0.01f), -1.0f);
		attachedBall->direction.Normalize();
		attachedBall->SetComboDamage(0);
		attachedBall = nullptr;
	}
}

void Paddle::Draw()
{
	NodeSprite::Draw();
	if (isFlashing)
	{
		float percentage = flashTimer / flashTime;
		int alpha = static_cast<int>(255 * percentage);

		SDL_Rect& dest = destRect; // Copy
		dest.x -= static_cast<int>(flashSizeIncrease.x * percentage / 2);
		dest.y -= static_cast<int>(flashSizeIncrease.y * percentage / 2);
		dest.w += static_cast<int>(flashSizeIncrease.x * percentage);
		dest.h += static_cast<int>(flashSizeIncrease.y * percentage);

		SDL_SetRenderDrawColor(game->renderer->renderer, 255, 255, 255, alpha);
		SDL_RenderCopy(game->renderer->renderer, GetTexture().get(), &cropRect, &dest);
		SDL_RenderFillRect(game->renderer->renderer, &destRect);
		SDL_SetRenderDrawColor(game->renderer->renderer, 255, 255, 255, 255);
	}
}

void Paddle::BallHitPaddle(Ball* ball)
{
	if (isFlashing)
	{
		FlashHitBall(ball);
	}
	else
	{
		pendingFlashBalls[ball] = game->GetTotalTime();
		//gameplay->combo->PaddleMissBall();
	}
}

void Paddle::FlashHitBall(Ball* ball)
{
	gameplay->combo->PaddleHit();
	flashHit = true;
	ball->SetComboDamage(gameplay->combo->GetCombo(), true);
}

void Paddle::FlashMissBall()
{
	gameplay->combo->PaddleMissBall();
}

void Paddle::FlashMiss()
{
	gameplay->combo->PaddleMiss();
}

float Paddle::GetHorizontalHitOffset(Vector2 hit)
{
	float offset = (hit.x - pos.x) / (currentRect->w / 2.0f);
	return clamp(offset, -1.0f, 1.0f);
}

void Paddle::EnlargePaddle()
{
	SetSize(LONG);
	sizeChanged = true;
	sizeChangedTimer = enlargeTime;
}

void Paddle::ShrinkPaddle()
{
	SetSize(SHORT);
	sizeChanged = true;
	sizeChangedTimer = shrinkTime;
}


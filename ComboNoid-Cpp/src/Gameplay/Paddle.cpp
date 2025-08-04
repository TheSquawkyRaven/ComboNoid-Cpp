#include "Paddle.h"
#include "../Game.h"
#include "Gameplay.h"
#include "Ball.h"
#include "Powerup.h"
#include "Combo.h"

Paddle::Paddle(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
	rLimit = game->renderX;
}

void Paddle::Init()
{
	IInput::Register(game);
	IUpdatable::Register(game);
	IDrawable::Register(game);

	gameplay->RegisterPaddle(this);

	shared_ptr<SDL_Texture> texture = game->renderer->LoadTexture("./assets/paddle.png");
	SetTexture(texture);

	SetSize(NORMAL);
}

void Paddle::OnDestroy()
{
	IInput::Unregister(game);
	IUpdatable::Unregister(game);
	IDrawable::Unregister(game);

	gameplay->UnregisterPaddle(this);
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

	if (pos.x < lLimit)
	{
		pos.x = lLimit;
	}
	else if (pos.x + destRect.w > rLimit)
	{
		pos.x = rLimit - destRect.w;
	}
	UpdateBall();

	PostUpdate();
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

void Paddle::PostUpdate()
{
	PlaceTexture(this);
	PlaceCol(this);
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
			printf("Unknown Paddle Size of %d\n", size);
			break;
	}
	CropTexture(*currentRect);

	// Collision size
	IRectCollidable::SetOffset(rectOffset.x, rectOffset.y);
	IRectCollidable::SetSize(rectSize.x, rectSize.y);

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
		xOffset = game->renderX / 2;
		w = -currentRect->w / 2;
	}
	pos.x = pos.x + xOffset + w;
	pos.y = game->renderY - currentRect->h - 32;
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

	attachedBall->pos.x = pos.x + (destRect.w - attachedBall->destRect.w) / 2;
	attachedBall->pos.y = pos.y + rectOffset.y -
		attachedBall->destRect.h + // Offset the ball to its size so it aligns on top of the paddle
		attachedBall->destRect.h - attachedBall->radius * 2; // Considers the size (NORMAL or LARGE) of the ball

	attachedBall->PostUpdate();

	if (spaceInput)
	{
		// Launch ball
		attachedBall->isAttached = false;
		attachedBall = nullptr;
	}
}

void Paddle::Draw()
{
	IDrawable::Draw();
	if (isFlashing)
	{
		float percentage = flashTimer / flashTime;
		int alpha = static_cast<int>(255 * percentage);
		SDL_SetRenderDrawColor(game->renderer->renderer, 255, 255, 255, alpha);
		SDL_Rect dest = destRect; // Copy
		dest.x -= flashSizeIncrease.x * percentage / 2;
		dest.y -= flashSizeIncrease.y * percentage / 2;
		dest.w += flashSizeIncrease.x * percentage;
		dest.h += flashSizeIncrease.y * percentage;
		SDL_RenderCopy(game->renderer->renderer, texture.get(), &srcRect, &dest);
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
	ball->SetComboDamage(gameplay->combo->GetCombo());
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
	float centerX = pos.x + size.x / 2;
	float offset = (hit.x - centerX) / (size.x / 2);
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


#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <memory>

#include "Node.h"

using namespace std;

class NodeSprite : public virtual Node
{
private:
	shared_ptr<SDL_Texture> texture = nullptr;

public:
	/// <summary>
	/// Rect dimensions to crop the original texture
	/// </summary>
	SDL_Rect cropRect{};
	/// <summary>
	/// Draw destination. All values are updated in the Draw() call
	/// </summary>
	SDL_Rect destRect{};

	SDL_Color color{ 255, 255, 255, 255 };

public:
	inline shared_ptr<SDL_Texture> GetTexture() const { return texture; }
	inline SDL_Rect& GetDestRect() { return destRect; }

	NodeSprite(Game* game);

	virtual void Draw() override;

	void SetTexture(shared_ptr<SDL_Texture> texture);

};

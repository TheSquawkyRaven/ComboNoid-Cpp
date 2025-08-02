#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY) : window(window), renderer(renderer)
{
	// Render Target - No matter what the window size is, we will always render to this target to make the world screen size consistent
    renderTarget = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        renderX, renderY
    );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void Renderer::Clear()
{
    // Set to render to the target renderer
    SDL_SetRenderTarget(renderer, renderTarget);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderClear(renderer);
}

void Renderer::Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect)
{
    // Each draw will be rendered to the render target
	SDL_RenderCopy(renderer, texture, srcRect, destRect);
}

void Renderer::Flush()
{
    // Render to the original renderer (not the target)
    SDL_SetRenderTarget(renderer, NULL);

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    float scale = min(winW / 800.0f, winH / 600.0f);

    int drawW = static_cast<int>(800 * scale);
    int drawH = static_cast<int>(600 * scale);

    int offsetX = (winW - drawW) / 2;
    int offsetY = (winH - drawH) / 2;

    SDL_Rect destRect = { offsetX, offsetY, drawW, drawH };

    // Clear screen (fill with black for letterboxing)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the render target scaled into the calculated rect
    SDL_RenderCopy(renderer, renderTarget, NULL, &destRect);

    SDL_RenderPresent(renderer);
}

shared_ptr<SDL_Texture> Renderer::LoadTexture(string path)
{
    if (textureMap.contains(path))
    {
        shared_ptr<SDL_Texture> loadedTexture = textureMap[path].lock();
        if (loadedTexture)
        {
			printf("Already loaded texture from %s\n", path.c_str());
            return loadedTexture;
        }
		printf("Texture %s was destroyed, reloading...\n", path.c_str());
        textureMap.erase(path);
    }
	SDL_Texture* texture_ = IMG_LoadTexture(renderer, path.c_str());
    if (texture_ == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }

    // Destruction uses SDL_DestroyTexture
    shared_ptr<SDL_Texture> texture = shared_ptr<SDL_Texture>(texture_, SDL_DestroyTexture);
    textureMap[path] = texture;

	return texture;
}
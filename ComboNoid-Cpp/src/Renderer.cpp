#include "Renderer.h"

Renderer::Renderer(SDL_Window* window, SDL_Renderer* renderer, int renderX, int renderY) : window(window), renderer(renderer)
{
    this->renderX = renderX;
    this->renderY = renderY;

	// Render Target - No matter what the window size is, we will always render to this target to make the world screen size consistent
    renderTarget = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        renderX, renderY
    );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
}

void Renderer::Clear() const
{
    // Set to render to the target renderer
    SDL_SetRenderTarget(renderer, renderTarget);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_RenderClear(renderer);
}

void Renderer::Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect) const
{
    // Each draw will be rendered to the render target
	SDL_RenderCopy(renderer, texture, srcRect, destRect);
}

void Renderer::Draw(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect, float angle) const
{
	SDL_Point center = { destRect->w / 2, destRect->h / 2 };
	SDL_RenderCopyEx(renderer, texture, srcRect, destRect, static_cast<double>(angle), &center, SDL_FLIP_NONE);
}

void Renderer::Flush()
{
    // Render to the original renderer (not the target)
    SDL_SetRenderTarget(renderer, NULL);

    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    float scale = min(winW / renderX, winH / renderY);

    int drawW = static_cast<int>(renderX * scale);
    int drawH = static_cast<int>(renderY * scale);

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
            return loadedTexture;
        }
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

shared_ptr<TTF_Font> Renderer::LoadFont(string path, int fontSize)
{
    if (fontMap.contains(path))
    {
        shared_ptr<TTF_Font> loadedFont = fontMap[path].lock();
        if (loadedFont)
        {
            return loadedFont;
        }
        fontMap.erase(path);
    }
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
    if (font == NULL)
    {
        printf("Failed to load font_ptr %s! SDL_TTF Error: %s\n", path.c_str(), TTF_GetError());
		return nullptr;
    }

	shared_ptr<TTF_Font> font_ptr = shared_ptr<TTF_Font>(font, TTF_CloseFont);
    fontMap[path] = font_ptr;

	return font_ptr;
}

shared_ptr<SDL_Texture> Renderer::LoadFontTexture(TTF_Font* font, int fontSize, const char* text, SDL_Color& color) const
{
    TTF_SetFontSize(font, fontSize);

    SDL_Surface* sdlSurface = TTF_RenderText_Solid(font, text, color);
    if (sdlSurface == nullptr)
    {
        printf("Failed to create surface from font_ptr! SDL_TTF_ERROR: %s\n", TTF_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sdlSurface);

    SDL_FreeSurface(sdlSurface);
    if (texture == nullptr)
    {
        printf("Failed to convert surface to texture! SDL_IMAGE_ERROR: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    //SDL_QueryTexture(texture, nullptr, nullptr, &texture->size.x, &texture->size.y);

    shared_ptr<SDL_Texture> texture_ptr = shared_ptr<SDL_Texture>(texture, SDL_DestroyTexture);

    return texture_ptr;
}

Vector2 Renderer::GetWindowCoordToRenderCoord(int windowX, int windowY) const
{
    int winW, winH;
    SDL_GetWindowSize(window, &winW, &winH);

    // Using int (floor) to allow integer scaling
    int scaleX = winW / renderX;
    int scaleY = winH / renderY;
    float scale = min(scaleX, scaleY);

    int drawW = (int)floor(renderX * scale);
    int drawH = (int)floor(renderY * scale);

    int offsetX = (winW - drawW) / 2;
    int offsetY = (winH - drawH) / 2;

    float rX = (windowX - offsetX) / scale;
    float rY = (windowY - offsetY) / scale;

    return Vector2(rX, rY);
}

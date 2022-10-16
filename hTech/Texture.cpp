#include "pch.h"
#include "Texture.h"

#include "Camera.h"
#include "Game.h"
#include "Log.h"

#include <SDL_stbimage.h>

Texture::Texture(std::string texture_path)
{
	Width = NULL;
	Height = NULL;
	mName == "";
	Create(texture_path);
}

Texture::~Texture()
{
	Width = NULL;
	Height = NULL;
	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = nullptr;
	}
}

SDL_Texture& Texture::GetSDLTexture()
{
	return *mTexture;
}

const std::string& Texture::GetLocation() const
{
	return mName;
}

void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation)
{
	Vector2 renderPosition = Camera::WorldToScreen(position);

	SDL_Rect destRect{};
	destRect.w = Width;
	destRect.h = Height;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);
	SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, rotation, NULL, SDL_FLIP_NONE);
}

void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation, Vector2 sourcePosition, Vector2 sourceDimensions)
{
	Vector2 renderPosition = Camera::WorldToScreen(position);

	SDL_Rect destRect{};
	destRect.w = sourceDimensions.X;
	destRect.h = sourceDimensions.Y;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);

	SDL_Rect srcRect{};
	srcRect.w = sourceDimensions.X;
	srcRect.h = sourceDimensions.Y;
	srcRect.x = (int)(sourcePosition.X) - (srcRect.w / 2);
	srcRect.y = (int)(sourcePosition.Y) - (srcRect.h / 2);

	SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, rotation, NULL, SDL_FLIP_NONE);
}


void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation, bool flipped)
{
	Vector2 renderPosition = Camera::WorldToScreen(position);

	SDL_Rect destRect{};
	destRect.w = Width;
	destRect.h = Height;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);

	if (flipped)
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, rotation, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, rotation, NULL, SDL_FLIP_HORIZONTAL);
}

void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation, Vector2 sourcePosition, Vector2 sourceDimensions, bool flipped)
{
	const Vector2 renderPosition = Camera::WorldToScreen(position);

	SDL_Rect destRect{};
	destRect.w = sourceDimensions.X;
	destRect.h = sourceDimensions.Y;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2.0f);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);
	
	SDL_Rect srcRect{};
	srcRect.w = sourceDimensions.X;
	srcRect.h = sourceDimensions.Y;
	srcRect.x = (int)(sourcePosition.X) - (srcRect.w / 2);
	srcRect.y = (int)(sourcePosition.Y) - (srcRect.h / 2);

	if(flipped)
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, (double)rotation, NULL, SDL_FLIP_NONE);
	else																   
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, (double)rotation, NULL, SDL_FLIP_HORIZONTAL);
}

void Texture::Create(std::string texture_path)
{
	assert(mTexture == nullptr);

	if (texture_path != "")
	{
		if (mTexture)
			SDL_DestroyTexture(mTexture);

		// Load image as SDL_Surface
		SDL_Surface* surface = STBIMG_Load(texture_path.c_str());
		if (surface == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load surface <" + texture_path + "> error : " + SDL_GetError() + "\n"));
		}

		assert(surface != nullptr);

		// SDL_Surface is just the raw pixels
		// Convert it to a hardware-optimzed texture so we can render it
		mTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
		if (mTexture == nullptr)
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));

		assert(mTexture != nullptr);

		SDL_QueryTexture(mTexture, NULL, NULL, &Width, &Height);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (mTexture == nullptr)
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));

		assert(mTexture != nullptr);

		mName = texture_path;
	}
}

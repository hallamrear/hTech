#include "pch.h"
#include "Texture.h"

#include "Camera.h"
#include "Game.h"
#include "Log.h"
#include "ProjectLoader.h"
#include <SDL_stbimage.h>

Texture::Texture(std::string texture_path)
{
	Width = NULL;
	Height = NULL;
	mName = "";
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
	destRect.w = (int)sourceDimensions.X;
	destRect.h = (int)sourceDimensions.Y;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);

	SDL_Rect srcRect{};
	srcRect.w = (int)sourceDimensions.X;
	srcRect.h = (int)sourceDimensions.Y;
	srcRect.x = (int)(sourcePosition.X) - (srcRect.w / 2);
	srcRect.y = (int)(sourcePosition.Y) - (srcRect.h / 2);

	SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, rotation, NULL, SDL_FLIP_NONE);
}


void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation, bool flipped)
{
	////ORIGINAL
	//Vector2 renderPosition = Camera::WorldToScreen(position);
	//SDL_Rect destRect{};
	//destRect.w = Width;
	//destRect.h = Height;
	//destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	//destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);

	//ALTERED
	Vector2 renderPosition = position;
	renderPosition.X -= (Width / 2);
	renderPosition.Y += (Height / 2);
	renderPosition = Camera::WorldToScreen(renderPosition);
	SDL_Rect destRect{};
	destRect.w = Width;
	destRect.h = Height;
	destRect.x = (int)(renderPosition.X);
	destRect.y = (int)(renderPosition.Y);

	if (flipped)
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, rotation, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, rotation, NULL, SDL_FLIP_HORIZONTAL);
}

void Texture::Render(SDL_Renderer& renderer, Vector2 position, float rotation, Vector2 sourcePosition, Vector2 sourceDimensions, bool flipped)
{
	const Vector2 renderPosition = Camera::WorldToScreen(position);

	SDL_Rect destRect{};
	destRect.w = (int)sourceDimensions.X;
	destRect.h = (int)sourceDimensions.Y;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);
	
	SDL_Rect srcRect{};
	srcRect.w = (int)sourceDimensions.X;
	srcRect.h = (int)sourceDimensions.Y;
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
			SDL_FreeSurface(surface);
			return;
		}

		// SDL_Surface is just the raw pixels
		// Convert it to a hardware-optimzed texture so we can render it
		mTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
		if (mTexture == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));
			SDL_DestroyTexture(mTexture);
			SDL_FreeSurface(surface);
			return;
		}

		SDL_QueryTexture(mTexture, NULL, NULL, &Width, &Height);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (mTexture == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));
			SDL_DestroyTexture(mTexture);
			SDL_FreeSurface(surface);
			return;
		}

		mName = texture_path;
	}
}

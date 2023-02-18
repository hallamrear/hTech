#include "pch.h"
#include "Texture.h"

#include "Camera.h"
#include "Game.h"
#include "Log.h"
#include "ProjectLoader.h"
#include "Transform.h"

#define SDL_STBIMAGE_IMPLEMENTATION
#include <SDL_stbimage.h>

Texture::Texture(std::string texture_path, std::string name)
{
	Width = NULL;
	Height = NULL;
	m_Name = "";
	m_Path = "";
	Create(texture_path, name);
}

Texture::~Texture()
{
	Width = NULL;
	Height = NULL;
	if (m_SDLTexture)
	{
		SDL_DestroyTexture(m_SDLTexture);
		m_SDLTexture = nullptr;
	}
}

SDL_Texture& Texture::GetSDLTexture()
{
	return *m_SDLTexture;
}

const std::string& Texture::GetLocation() const
{
	return m_Path;
}

const std::string& Texture::GetName() const
{
	return m_Name;
}

void Texture::Render(SDL_Renderer& renderer, const Transform& transform)
{
	Vector2 renderPosition = Camera::WorldToScreen(transform.Position);

	SDL_Rect destRect{};
	destRect.w = Width;
	destRect.h = Height;
	destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);
	SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, transform.Rotation, NULL, SDL_FLIP_NONE);
}

void Texture::Render(SDL_Renderer& renderer, const Transform& transform, const Vector2& sourcePosition, const Vector2& sourceDimensions)
{
	Vector2 renderPosition = Camera::WorldToScreen(transform.Position);

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

	SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, transform.Rotation, NULL, SDL_FLIP_NONE);
}


void Texture::Render(SDL_Renderer& renderer, const Transform& transform, const bool& flipped)
{
	////ORIGINAL
	//Vector2 renderPosition = Camera::WorldToScreen(position);
	//SDL_Rect destRect{};
	//destRect.w = Width;
	//destRect.h = Height;
	//destRect.x = (int)(renderPosition.X) - (destRect.w / 2);
	//destRect.y = (int)(renderPosition.Y) - (destRect.h / 2);

	//ALTERED
	Vector2 renderPosition = transform.Position;
	renderPosition.X -= (Width / 2);
	renderPosition.Y += (Height / 2);
	renderPosition = Camera::WorldToScreen(renderPosition);
	SDL_Rect destRect{};
	destRect.w = Width;
	destRect.h = Height;
	destRect.x = (int)(renderPosition.X);
	destRect.y = (int)(renderPosition.Y);

	if (flipped)
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, transform.Rotation, NULL, SDL_FLIP_HORIZONTAL);
	else															   
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), NULL, &destRect, transform.Rotation, NULL, SDL_FLIP_NONE);
}

void Texture::Render(SDL_Renderer& renderer, const Transform& transform, const Vector2& sourcePosition, const Vector2& sourceDimensions, const bool& flipped)
{
	const Vector2 renderPosition = Camera::WorldToScreen(transform.Position);

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
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, (double)transform.Rotation, NULL, SDL_FLIP_NONE);
	else																   
		SDL_RenderCopyEx(&renderer, &GetSDLTexture(), &srcRect, &destRect, (double)transform.Rotation, NULL, SDL_FLIP_HORIZONTAL);
}

void Texture::Create(std::string texture_path, std::string name)
{
	assert(m_SDLTexture == nullptr);

	if (texture_path != "")
	{
		if (m_SDLTexture)
			SDL_DestroyTexture(m_SDLTexture);

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
		m_SDLTexture = SDL_CreateTextureFromSurface(Game::Renderer, surface);
		if (m_SDLTexture == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));
			SDL_DestroyTexture(m_SDLTexture);
			SDL_FreeSurface(surface);
			return;
		}

		SDL_QueryTexture(m_SDLTexture, NULL, NULL, &Width, &Height);
		// Don't need the orignal texture, release the memory
		SDL_FreeSurface(surface);

		if (m_SDLTexture == nullptr)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));
			SDL_DestroyTexture(m_SDLTexture);
			SDL_FreeSurface(surface);
			return;
		}

		m_Path = texture_path;
		m_Name = name;
	}
}

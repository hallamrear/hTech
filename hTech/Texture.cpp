#include "pch.h"
#include "Texture.h"

#include "Camera.h"
#include "Game.h"
#include "Log.h"
#include "ProjectLoader.h"
#include "Transform.h"

#define SDL_STBIMAGE_IMPLEMENTATION
#include <SDL_stbimage.h>
#include "OriginalRenderer.h"

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

SDL_Texture& Texture::GetSDLTexture() const
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

		//todo : texture needs to be abstracted so this needs to be replaced.
		OriginalRenderer& sdlRenderer = (OriginalRenderer&)(Game::GetRenderer());
		m_SDLTexture = SDL_CreateTextureFromSurface(sdlRenderer.GetAPIRenderer(), surface);
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

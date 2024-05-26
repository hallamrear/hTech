#include "pch.h"
#include "Rendering/OriginalTexture.h"
#include "Rendering/OriginalRenderer.h"
#include <System/Console.h>
#include <System/Engine.h>

#define STB_IMAGE_IMPLEMENTATION
#include <External/STB/include/stb_image.h>

OriginalTexture::OriginalTexture(const std::string& texture_path, const std::string& name)
	: ITexture(texture_path, name)
{
	m_SDLTexture = nullptr;
	Create(texture_path, name);
}

OriginalTexture::~OriginalTexture()
{

}

bool OriginalTexture::Create(const std::string& texture_path, const std::string& name)
{
	assert(m_SDLTexture == nullptr);

	if (texture_path != "")
	{
		if (m_SDLTexture)
			SDL_DestroyTexture(m_SDLTexture);

		//Load file
		int channelCount;
		unsigned char* data = stbi_load(texture_path.c_str(), &m_Width, &m_Height, &channelCount, 4);

		if (!data)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load image <" + texture_path + "> \n"));
			stbi_image_free(data);
			return false;
		}

		//Create surface
		OriginalRenderer& sdlRenderer = (OriginalRenderer&)(Engine::GetRenderer());
		SDL_Renderer* renderer = sdlRenderer.GetAPIRenderer();
		m_SDLTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, m_Width, m_Height);
		SDL_UpdateTexture(m_SDLTexture, NULL, data, m_Width * 4);
		stbi_image_free(data);

		if (m_SDLTexture == nullptr)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, std::string("Failed to load texture <" + texture_path + "> error : " + SDL_GetError() + "\n"));
			SDL_DestroyTexture(m_SDLTexture);
			return false;
		}

		m_Path = texture_path;
		m_Name = name;

		return true;
	}
}

bool OriginalTexture::Destroy()
{
	if (m_SDLTexture)
	{
		SDL_DestroyTexture(m_SDLTexture);
		m_SDLTexture = nullptr;
	}

	return true;
}

SDL_Texture& OriginalTexture::GetSDLTexture() const
{
	return *m_SDLTexture;
}

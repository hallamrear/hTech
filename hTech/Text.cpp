#include "pch.h"
#include "Text.h"
#include "Log.h"
#include "Transform.h"
#include "Game.h"

TTF_Font* Text::m_Font = nullptr;

bool Text::CreateTTFFontAsset(const int& fontSize, const std::string& fontLocation)
{
	if (m_Font)
	{
		DestroyTTFFontAsset();
	}

	m_Font = TTF_OpenFont(fontLocation.c_str(), (int)fontSize);
	if (!m_Font)
	{		
		std::string str = TTF_GetError();
		Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
		Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		
		DestroyTTFFontAsset();
		return false;
	}

	return false;
}

bool Text::DestroyTTFFontAsset()
{
	if (m_Font != nullptr)
	{
		TTF_CloseFont(m_Font);
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Font destroyed.");
		return true;
	}

	return false;
}

void Text::CreateTextTexture()
{
	if (m_IsDirty)
	{
		if (m_Data == "")
			return;

		if (mTextTexture != nullptr)
		{
			DestroyTextTexture();
		}

		SDL_Renderer& renderer = const_cast<SDL_Renderer&>(*Game::Renderer);
		SDL_Surface* textSurface = nullptr;
		SDL_Color color = { m_Colour.R, m_Colour.G, m_Colour.B, m_Colour.A };

		textSurface = TTF_RenderUTF8_Blended_Wrapped(m_Font, m_Data.c_str(), color, m_WrapWidth);

		if (!textSurface)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO RENDER TEXT");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
			return;
		}

		mTextTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
		SDL_QueryTexture(mTextTexture, NULL, NULL, &m_Width, &m_Height);

		SDL_FreeSurface(textSurface);
		textSurface = nullptr;
	}
}

void Text::DestroyTextTexture()
{
	SDL_DestroyTexture(mTextTexture);
}

Text::Text(Vector2 Position, std::string text, Colour colour)
{
	m_IsDirty = true;
	m_Colour = colour;
	m_Data = text;

	if (m_Font == nullptr)
	{
		CreateTTFFontAsset();
	}

}

Text::~Text()
{
	if (m_Font != nullptr)
	{
		DestroyTTFFontAsset();
	}

}

void Text::Update(float DeltaTime)
{
	if (m_IsDirty)
	{
		CreateTextTexture();

		if (mTextTexture)
		{
			m_IsDirty = false;
		}
	}
}

void Text::Render(SDL_Renderer& renderer)
{
	if (mTextTexture)
	{
		SDL_Rect destRect{};
		destRect.x = (int)m_Position.X;
		destRect.y = (int)m_Position.Y;
		destRect.w = m_Width;
		destRect.h = m_Height;
		SDL_RenderCopy(&renderer, mTextTexture, NULL, &destRect);
	}
}

void Text::SetWrapWidthInPixels(int width)
{
	m_WrapWidth = width;
}

void Text::SetPosition(const Vector2& screenSpacePosition)
{
	m_Position = screenSpacePosition;
}

void Text::SetString(const std::string& str)
{
	m_Data = str;
	m_IsDirty = true;
}

void Text::SetColour(const Colour& colour)
{
	m_Colour = colour;
	m_IsDirty = true;
}

void Text::SetString(const char* str)
{
	SetString(std::string(str));
}

const Vector2 Text::GetTextureSize() const
{
	return Vector2((float)m_Width, (float)m_Height);
}

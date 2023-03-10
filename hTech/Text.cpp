#include "pch.h"
#include "Text.h"
#include "Console.h"
#include "Transform.h"
#include "Engine.h"
#include "IRenderer.h"
#include "OriginalRenderer.h"

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
		Console::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
		Console::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		
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
		Console::LogMessage(LogLevel::LOG_MESSAGE, "Font destroyed.");
		return true;
	}

	return false;
}

void Text::CreateTextTexture()
{
	return;
	
	//TODO : UNCOMMENT WHEN REDOING TEXT;
	/*
	if (m_IsDirty)
	{
		if (m_Data == "")
			return;

		if (mTextTexture != nullptr)
		{
			DestroyTextTexture();
		}

		SDL_Renderer& renderer = const_cast<SDL_Renderer&>(*Engine::GetRenderer());
		SDL_Surface* textSurface = nullptr;
		SDL_Color color = { m_Colour.R, m_Colour.G, m_Colour.B, m_Colour.A };

		textSurface = TTF_RenderUTF8_Blended_Wrapped(m_Font, m_Data.c_str(), color, m_WrapWidth);

		if (!textSurface)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, "FAILED TO RENDER TEXT");
			Console::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
			return;
		}

		mTextTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
		SDL_QueryTexture(mTextTexture, NULL, NULL, &m_Width, &m_Height);

		SDL_FreeSurface(textSurface);
		textSurface = nullptr;
	}*/
}

void Text::DestroyTextTexture()
{
	SDL_DestroyTexture(mTextTexture);
}

Text::Text(Transform transform, std::string text, Colour colour)
{
	m_Transform = transform;
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

void Text::Render(IRenderer& renderer)
{
	if (mTextTexture)
	{
		SDL_Rect destRect =
		{
			(int)m_Transform.Position.X,
			(int)m_Transform.Position.Y,
			m_Width,
			m_Height
		};

		//todo : text needs to be abstracted to avoid using SDL_Renderer. 
		//it cant use the existing render functions because it uses SDL_Textures directly rather than 
		//my texture class.

		OriginalRenderer& ref = (OriginalRenderer&)Engine::GetRenderer();
		SDL_Renderer* r = ref.GetAPIRenderer();
		SDL_RenderCopy(r, mTextTexture, nullptr, &destRect);

		/*renderer.Render_Texture(mTextTexture, m_Transform, RENDER_LAYER::DEFAULT, false);
		renderer.Render_Texture(
			*mTextTexture, 
			m_Transform, 
			RENDER_LAYER::FOREGROUND,
			nullptr,
			nullptr, 
			&destRect,
			false);*/

	}
}

void Text::SetWrapWidthInPixels(int width)
{
	m_WrapWidth = width;
}

void Text::SetPosition(const Vector2& screenSpacePosition)
{
	m_Transform.Position = screenSpacePosition;
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

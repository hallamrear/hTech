#include "pch.h"
#include "Text.h"
#include "Log.h"
#include "Transform.h"
#include "Game.h"

TTF_Font* Text::mFont = nullptr;

bool Text::CreateTTFFontAsset(const int& fontSize, const std::string& fontLocation)
{
	if (mFont)
	{
		DestroyTTFFontAsset();
	}

	mFont = TTF_OpenFont(fontLocation.c_str(), (int)fontSize);
	if (!mFont)
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
	if (mFont != nullptr)
	{
		TTF_CloseFont(mFont);
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Font destroyed.");
		return true;
	}

	return false;
}

void Text::CreateTextTexture()
{
	if (mIsDirty)
	{
		if (mData == "")
			return;

		if (mTextTexture != nullptr)
		{
			DestroyTextTexture();
		}

		SDL_Renderer& renderer = const_cast<SDL_Renderer&>(*Game::Renderer);
		SDL_Surface* textSurface = nullptr;
		SDL_Color color = { mColour.R, mColour.G, mColour.B, mColour.A };

		textSurface = TTF_RenderUTF8_Blended_Wrapped(mFont, mData.c_str(), color, mWrapWidth);

		if (!textSurface)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO RENDER TEXT");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
			return;
		}

		mTextTexture = SDL_CreateTextureFromSurface(&renderer, textSurface);
		SDL_QueryTexture(mTextTexture, NULL, NULL, &mTextWidth, &mTextHeight);

		SDL_FreeSurface(textSurface);
		textSurface = nullptr;
	}
}

void Text::DestroyTextTexture()
{
	SDL_DestroyTexture(mTextTexture);
}

Text::Text(Vector2 position, std::string text, Colour colour)
{
	mIsDirty = true;
	mColour = colour;
	mData = text;

	if (mFont == nullptr)
	{
		CreateTTFFontAsset();
	}

}

Text::~Text()
{
	if (mFont != nullptr)
	{
		DestroyTTFFontAsset();
	}

}

void Text::Update(float DeltaTime)
{
	if (mIsDirty)
	{
		CreateTextTexture();

		if (mTextTexture)
		{
			mIsDirty = false;
		}
	}
}

void Text::Render(SDL_Renderer& renderer)
{
	if (mTextTexture)
	{
		SDL_Rect destRect{};
		destRect.x = (int)mPosition.X;
		destRect.y = (int)mPosition.Y;
		destRect.w = mTextWidth;
		destRect.h = mTextHeight;
		SDL_RenderCopy(&renderer, mTextTexture, NULL, &destRect);
	}
}

void Text::SetWrapWidthInPixels(int width)
{
	mWrapWidth = width;
}

void Text::SetPosition(const Vector2& screenSpacePosition)
{
	mPosition = screenSpacePosition;
}

void Text::SetString(const std::string& str)
{
	mData = str;
	mIsDirty = true;
}

void Text::SetColour(const Colour& colour)
{
	mColour = colour;
	mIsDirty = true;
}

void Text::SetString(const char* str)
{
	SetString(std::string(str));
}

const Vector2 Text::GetTextureSize() const
{
	return Vector2((float)mTextWidth, (float)mTextHeight);
}

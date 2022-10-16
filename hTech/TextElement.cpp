#include "pch.h"
#include "TextElement.h"
#include "Log.h"
#include "Game.h"
#include "Texture.h"
#include "Camera.h"

TextElement::TextElement(std::string string, float size, Colour colour)	: Entity("")
{
	mTextTexture = nullptr;
	mIsShowing = true;
	mData = string;
	mColour = colour;
	mFontSize = size;
	mTextWidth = NULL;
	mTextHeight = NULL;
	mIsDirty = true;
}

TextElement::~TextElement()
{
	mIsDirty = false;
}

void TextElement::DestroyTexture()
{
	SDL_DestroyTexture(mTextTexture);
}

void TextElement::CreateTexture()
{
	if (mData != "")
	{
		//This should not be getting recreated each frame
		TTF_Font* font;
		font = TTF_OpenFont("Assets/arial.ttf", (int)mFontSize);
		if (!font)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "FAILED TO LOAD FONT");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}

		SDL_Renderer& renderer = const_cast<SDL_Renderer&>(*Game::Renderer);
		SDL_Surface* textSurface = nullptr;
		//// Set color to black
		SDL_Color color = { mColour.R, mColour.G, mColour.B, mColour.A };

		textSurface = TTF_RenderText_Solid(font, mData.c_str(), color);

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

		TTF_CloseFont(font);
		font = nullptr;
	}
}

void TextElement::Update(double deltaTime)
{
	if(mIsDirty)
	{
		DestroyTexture();
		CreateTexture();

		if (mTextTexture)
		{
			mIsDirty = false;
		}
	}
}

void TextElement::Render()
{
	if(mIsShowing)
	{
		if(mTextTexture)
		{
			SDL_Rect destRect{};
			destRect.w = mTextWidth;
			destRect.h = mTextHeight;
			Vector2 position = Camera::WorldToScreen(Vector2((GetTransform().Position.X) - (destRect.w / 2), (GetTransform().Position.Y) - (destRect.h / 2)));
			destRect.x = static_cast<int>(position.X);
			destRect.y = static_cast<int>(position.Y);
			SDL_RenderCopyEx(Game::Renderer, mTextTexture, NULL, &destRect, GetTransform().Rotation, NULL, SDL_FLIP_NONE);
		}
	}
}

void TextElement::SetShowing(bool state)
{
	mIsShowing = state;
}

void TextElement::SetString(std::string str)
{
	mData = str;
	mIsDirty = true;
}

void TextElement::SetColour(Colour colour)
{
	mColour = colour;
}

void TextElement::SetString(const char* str)
{
	SetString(std::string(str));
}

void TextElement::SetPosition(Vector2 worldPosition)
{
	GetTransform().Position = worldPosition;
}

Vector2 TextElement::GetTextureSize()
{
	return Vector2((float)mTextWidth, (float)mTextHeight);
}
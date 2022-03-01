#include "pch.h"
#include "TextElement.h"
#include "Log.h"
#include "Game.h"
#include "Texture.h"
#include "Camera.h"

//todo: add font to constuctor
//todo: create font cache

TextElement::TextElement(Transform transform, std::string string, float size, Colour colour)
	: Entity("", mTransform)
{
	mTextTexture = nullptr;
	mIsShowing = true;
	mData = string;
	mColour = colour;
	mFontSize = size;
	mTransform = transform;
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
		font = TTF_OpenFont("BadComic-Regular.ttf", (int)mFontSize);
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
			Vector2f position = Camera::WorldToScreen(Vector2f((mTransform.Position.X) - (destRect.w / 2), (mTransform.Position.Y) - (destRect.h / 2)));
			destRect.x = static_cast<int>(position.X);
			destRect.y = static_cast<int>(position.Y);
			SDL_RenderCopyEx(Game::Renderer, mTextTexture, NULL, &destRect, mTransform.Rotation, NULL, SDL_FLIP_NONE);
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

void TextElement::SetPosition(Vector2f worldPosition)
{
	mTransform.Position = worldPosition;
}

Vector2f TextElement::GetTextureSize()
{
	return Vector2f((float)mTextWidth, (float)mTextHeight);
}
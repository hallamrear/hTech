#include "pch.h"
#ifdef DEFINE_OLD_UI
#include "UI_Button.h"
#include "Colour.h"
#include "Text.h"

UI_Button::UI_Button(UI_Panel rect, std::string text, std::function<void()> func) : UI_Element(rect)
{
	mWasPressedLastFrame = false;
	mIsPressed = false;
	mFunction = func;

	mPressedColour.R = mPanel.BackgroundColour.R - (mPanel.BackgroundColour.R / 8);
	mPressedColour.G = mPanel.BackgroundColour.G - (mPanel.BackgroundColour.G / 8);
	mPressedColour.B = mPanel.BackgroundColour.B - (mPanel.BackgroundColour.B / 8);
	mPressedColour.A = mPanel.BackgroundColour.A - (mPanel.BackgroundColour.A / 8);

	Vector2 screenPos;
	mText = new Text(screenPos, text, rect.TextColour);
	mText->SetWrapWidthInPixels(mPanel.W * UI_TILE_SIZE);
	screenPos.X = mPanel.X * UI_TILE_SIZE + (mText->GetTextureSize().X / 2);
	screenPos.Y = mPanel.Y * UI_TILE_SIZE - (mText->GetTextureSize().Y / 2);
	mText->SetPosition(screenPos);
}

UI_Button::~UI_Button()
{

}

void UI_Button::Update(float DeltaTime)
{
	if (mIsPressed)
	{
		if (mWasPressedLastFrame)
		{
			mIsPressed = false;
			mWasPressedLastFrame = false;
		}
		else
		{
			mWasPressedLastFrame = true;
		}
	}

	mText->Update(DeltaTime);
}

void UI_Button::Render(SDL_Renderer& renderer)
{
	if (mIsPressed)
	{
		DrawPanel(renderer, Colour(mPressedColour.R, mPressedColour.G, mPressedColour.B, mPressedColour.A), true);
		DrawPanel(renderer, Colour(mPanel.BackgroundColour.R, mPanel.BackgroundColour.G, mPanel.BackgroundColour.B, mPanel.BackgroundColour.A), false);
	}
	else
	{
		DrawPanel(renderer, Colour(mPanel.BackgroundColour.R, mPanel.BackgroundColour.G, mPanel.BackgroundColour.B, mPanel.BackgroundColour.A), true);
	}

	mText->Render(renderer);
}

void UI_Button::OnClick()
{
	if (mIsPressed == false)
	{
		mFunction();
		mIsPressed = true;
	}
}

void UI_Button::SetPressedColour(Colour colour)
{
	mPressedColour = colour;
}

#endif
#include "pch.h"
#include "UI_Button.h"
#include "Colour.h"
#include "Text.h"

UI_Button::UI_Button(UI_Panel rect, std::string text, std::function<void()> func) : UI_Element(rect)
{
	mWasPressedLastFrame = false;
	mIsPressed = false;
	mFunction = func;

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
		DrawPanel(renderer, Colour(64, 64, 64, 255), true);
	}
	else
	{
		DrawPanel(renderer, Colour(200, 200, 200, 255), true);
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

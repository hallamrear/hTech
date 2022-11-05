#include "pch.h"
#include "UI_TextPanel.h"
#include "Text.h"

UI_TextPanel::UI_TextPanel(UI_Panel rect, std::string string) : UI_Element(rect)
{
	float size = UI_TILE_SIZE;
	Vector2 screenPos;
	mText = new Text(screenPos, string, Colour(0, 0, 0, 255));
	mText->SetWrapWidthInPixels(mPanel.W * UI_TILE_SIZE);
	screenPos.X = mPanel.X * UI_TILE_SIZE + (mText->GetTextureSize().X / 2);
	screenPos.Y = mPanel.Y * UI_TILE_SIZE - (mText->GetTextureSize().Y / 2);
	mText->SetPosition(screenPos);
}

UI_TextPanel::~UI_TextPanel()
{
	delete mText;
	mText = nullptr;
}

void UI_TextPanel::Update(float DeltaTime)
{
	mText->Update(DeltaTime);
}

void UI_TextPanel::Render(SDL_Renderer& renderer)
{
	DrawPanel(renderer, mPanel.BackgroundColour);
	mText->Render(renderer);
}
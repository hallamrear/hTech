#include "pch.h"
#include "UI_Element.h"
#include "UI.h"
#include "Text.h"
#include "Camera.h"

UI_Element::UI_Element(PanelRect rect)
{
	mPanel = rect;
	mText = nullptr;
}

UI_Element::~UI_Element()
{
	delete mText;
	mText = nullptr;
}

void UI_Element::DrawPanel(SDL_Renderer& renderer, Colour color, bool drawFilled)
{
	SDL_Rect panel{};
	panel.x = mPanel.X * UI_TILE_SIZE;
	panel.y = mPanel.Y * UI_TILE_SIZE;
	panel.w = mPanel.W * UI_TILE_SIZE;
	panel.h = mPanel.H * UI_TILE_SIZE;

	SDL_SetRenderDrawColor(&renderer, color.R, color.G, color.B, color.A);

	if (drawFilled)
	{
		SDL_RenderFillRect(&renderer, &panel);
	}
	else
	{
		SDL_RenderDrawRect(&renderer, &panel);
	}

}

void UI_Element::Update(float DeltaTime)
{

}

void UI_Element::Render(SDL_Renderer& renderer)
{

}

void UI_Element::OnClick()
{

}

ExampleUIPanel::ExampleUIPanel(PanelRect rect, std::string string) : UI_Element(rect)
{
	float size = UI_TILE_SIZE;
	Vector2 screenPos;
	mText = new Text(screenPos, string, Colour(0, 0, 0, 255));
	screenPos.X = mPanel.X * UI_TILE_SIZE + (mText->GetTextureSize().X / 2);
	screenPos.Y = mPanel.Y * UI_TILE_SIZE - (mText->GetTextureSize().Y / 2);
	mText->SetPosition(screenPos);
}

ExampleUIPanel::~ExampleUIPanel()
{
	delete mText;
	mText = nullptr;
}

void ExampleUIPanel::Update(float DeltaTime)
{
	mText->Update(DeltaTime);
}

void ExampleUIPanel::Render(SDL_Renderer& renderer)
{
	DrawPanel(renderer, mPanel.BackgroundColour);
	mText->Render(renderer);
}
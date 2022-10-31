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
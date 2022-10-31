#pragma once
#include "Text.h"
#include "UI_Element.h"

template<class R>
class UI_VariableTracker :
    public UI_Element
{
private:
    const R& mReference;
    std::string mAdditionalText;

public:
    UI_VariableTracker(PanelRect rect, const R& reference, std::string additionalText);
    ~UI_VariableTracker();

    void Update(float DeltaTime);
    void Render(SDL_Renderer& renderer);
};

template<class R>
inline UI_VariableTracker<R>::UI_VariableTracker(PanelRect rect, const R& variable, std::string additionalText)
	: UI_Element(rect), mReference(variable)
{
	mAdditionalText = additionalText;
	Vector2 screenPos;
	mText = new Text(screenPos, "", rect.TextColour);
	screenPos.X = mPanel.X * UI_TILE_SIZE + (mText->GetTextureSize().X / 2);
	screenPos.Y = mPanel.Y * UI_TILE_SIZE - (mText->GetTextureSize().Y / 2);
	mText->SetPosition(screenPos);
}

template<class R>
inline UI_VariableTracker<R>::~UI_VariableTracker()
{
	mAdditionalText = "";
}

template<class R>
inline void UI_VariableTracker<R>::Update(float DeltaTime)
{
	mText->SetString(mAdditionalText + std::to_string(mReference));
	mText->Update(DeltaTime);
}

template<class R>
inline void UI_VariableTracker<R>::Render(SDL_Renderer& renderer)
{
	DrawPanel(renderer, mPanel.BackgroundColour, true);
	mText->Render(renderer);
}
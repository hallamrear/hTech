#pragma once
#ifdef DEFINE_OLD_UI
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)


#include "Text.h"
#include "UI_Element.h"
#include "Entity.h"

template<class R>
class HTECH_FUNCTION_EXPORT UI_VariableTracker :
    public UI_Element
{
private:
    const R& mReference;
    std::string mAdditionalText;

public:
    UI_VariableTracker(UI_Panel rect, const R& reference, std::string additionalText);

    ~UI_VariableTracker();

    void Update(float DeltaTime);
    void Render(IRenderer& renderer);
};

template<class R>
inline UI_VariableTracker<R>::UI_VariableTracker(UI_Panel rect, const R& variable, std::string additionalText)
	: UI_Element(rect), mReference(variable)
{
	mAdditionalText = additionalText;
	Vector2 screenPos;
	mText = new Text(screenPos, "", rect.TextColour);
	mText->SetWrapWidthInPixels(mPanel.W * UI_TILE_SIZE);
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
inline void UI_VariableTracker<R>::Render(IRenderer& renderer)
{
	DrawPanel(renderer, mPanel.BackgroundColour, true);
	mText->Render(renderer);
}

template<>
inline UI_VariableTracker<Entity>::UI_VariableTracker(UI_Panel rect, const Entity& variable, std::string additionalText) : UI_Element(rect), mReference(variable)
{
	mAdditionalText = additionalText;
	Vector2 screenPos;
	mText = new Text(screenPos, "", rect.TextColour);
	mText->SetWrapWidthInPixels(mPanel.W * UI_TILE_SIZE);
	screenPos.X = mPanel.X * UI_TILE_SIZE + (mText->GetTextureSize().X / 2);
	screenPos.Y = mPanel.Y * UI_TILE_SIZE - (mText->GetTextureSize().Y / 2);
	mText->SetPosition(screenPos);
}

template<>
inline void UI_VariableTracker<Entity>::Update(float DeltaTime)
{
	std::string str = mReference.GetName() + "\n";
	str += "Is Alive? : " + std::to_string(mReference.GetIsAlive()) + "\n";
	str += "Is Enabled? :" + std::to_string(mReference.IsEnabled) + "\n";
	str += "Notes: " + mAdditionalText + "\n";
	mText->SetString(str);
	mText->Update(DeltaTime);
}

template<>
inline void UI_VariableTracker<Entity>::Render(IRenderer& renderer)
{
	DrawPanel(renderer, mPanel.BackgroundColour, true);
	mText->Render(renderer);
}
#endif
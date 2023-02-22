#pragma once
#ifdef DEFINE_OLD_UI
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "UI_Element.h"
#include <functional>

class HTECH_FUNCTION_EXPORT UI_Button : public UI_Element
{
private:
	bool mWasPressedLastFrame;
	bool mIsPressed;
	std::function<void()> mFunction;
	Colour mPressedColour;

public:
	UI_Button(UI_Panel rect, std::string text, std::function<void()> func);
	~UI_Button();

	void Update(float DeltaTime);
	void Render(IRenderer& renderer);

	void OnClick();
	void SetPressedColour(Colour colour);
};

#endif

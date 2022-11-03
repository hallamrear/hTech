#pragma once
#include "UI_Element.h"
#include <functional>

class UI_Button : public UI_Element
{
private:
	bool mWasPressedLastFrame;
	bool mIsPressed;
	std::function<void()> mFunction;

public:
	UI_Button(PanelRect rect, std::string text, std::function<void()> func);
	~UI_Button();

	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer);

	void OnClick();
};

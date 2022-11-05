#pragma once
#include <string>
#include "UI_PanelRect.h"

struct SDL_Renderer;
class Text;

class UI_Element
{
protected:
	PanelRect mPanel;
	Text* mText;
	bool mInFocus;

	UI_Element(PanelRect rect);
	virtual ~UI_Element() = 0;

	void DrawPanel(SDL_Renderer& renderer, Colour color, bool drawFilled = true);

public:
	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);
	virtual void OnClick();

	bool GetInFocus() const;
	void SetInFocus(bool state);
};
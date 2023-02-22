#pragma once
#ifdef DEFINE_OLD_UI
#define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include <string>
#include "UI_Panel.h"

struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Text;

class HTECH_FUNCTION_EXPORT UI_Element
{
protected:
	UI_Panel mPanel;
	Text* mText;
	bool mInFocus;

	UI_Element(UI_Panel rect);
	virtual ~UI_Element() = 0;

	void DrawPanel(SDL_Renderer& renderer, Colour color, bool drawFilled = true);

public:
	UI_Panel& GetPanel();

	virtual void Update(float DeltaTime);
	virtual void Render(IRenderer& renderer);
	virtual void OnClick();

	bool GetInFocus() const;
	void SetInFocus(bool state);
};
#endif
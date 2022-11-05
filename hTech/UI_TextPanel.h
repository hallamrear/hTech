#pragma once
#include "UI_Element.h"

class UI_TextPanel : public UI_Element
{
public:
	UI_TextPanel(UI_Panel rect, std::string string);
	~UI_TextPanel();

	virtual void Update(float DeltaTime);
	virtual void Render(SDL_Renderer& renderer);
};
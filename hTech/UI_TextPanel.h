#pragma once
#ifdef DEFINE_OLD_UI

 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include "UI_Element.h"

class HTECH_FUNCTION_EXPORT UI_TextPanel : public UI_Element
{
public:
	UI_TextPanel(UI_Panel rect, std::string string);
	~UI_TextPanel();

	virtual void Update(float DeltaTime);
	virtual void Render(IRenderer& renderer);
};

#endif
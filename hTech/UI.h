#pragma once
#include "UI_Element.h"
#include "UI_VariableTracker.h"

class Camera;
class Transform;
class UI_Panel;
class UI_Button;
class UI_TextPanel;

class UI
{
private:	
	std::vector<SDL_Point> mUIGridLayoutPoints;
	unsigned int mUIGridColumnCount;
	unsigned int mUIGridRowCount;
	std::vector<UI_Element*> mUIElements;
	UI_Element*** mUIMap;
	UI_Element* mFocusedObject;

	static UI* mInstance;
	const Vector2& mWindowDimensions;

	void AddUIElementToScreenMap(UI_Element* element, UI_Panel panel);

protected:
	UI();
	~UI();

	static UI* Get();
	void Update_Impl(float DeltaTime);
	void Render_Impl(SDL_Renderer& renderer);

	template<class T>
	UI_VariableTracker<T>* CreateVariableTracker_Impl(UI_Panel panel, T& reference, std::string additionalText);

	UI_TextPanel* CreateTextPanel_Impl(UI_Panel panel, std::string string);
	UI_Button* CreateButton_Impl(UI_Panel panel, std::string text, std::function<void()> function);
	bool OnMouseClick_Impl();

public:
	static bool OnMouseClick();
	static UI_Button* CreateButton(UI_Panel panel, std::string text, std::function<void()> function);
	static UI_TextPanel* CreateTextPanel(UI_Panel panel, std::string string);

	template<class T>
	static UI_VariableTracker<T>* CreateVariableTracker(UI_Panel panel, T& reference, std::string additionalText);

	static void Update(float DeltaTime);
	static void Render(SDL_Renderer& renderer);
};

template<class T>
inline  UI_VariableTracker<T>* UI::CreateVariableTracker(UI_Panel panel, T& reference, std::string additionalText)
{
	return Get()->CreateVariableTracker_Impl(panel, reference, additionalText);
}

template<class T>
inline  UI_VariableTracker<T>* UI::CreateVariableTracker_Impl(UI_Panel panel, T& reference, std::string additionalText)
{
	UI_VariableTracker<T>* tracker = new UI_VariableTracker<T>(panel, reference, additionalText);
	AddUIElementToScreenMap(tracker, panel);
	return tracker;
}
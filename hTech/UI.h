#pragma once
#include "UI_Element.h"
#include "UI_VariableTracker.h"

class Camera;
class Transform;
class PanelRect;

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

	void AddUIElementToScreenMap(UI_Element* element, PanelRect panel);

protected:
	UI();
	~UI();

	static UI* Get();
	void Update_Impl(float DeltaTime);
	void Render_Impl(SDL_Renderer& renderer);

	template<class T>
	void CreateVariableTracker_Impl(PanelRect panel, T& reference, std::string additionalText);

	void CreateExamplePanel_Impl(PanelRect panel, std::string string);
	void CreateButton_Impl(PanelRect panel, std::string text, std::function<void()> function);
	bool OnMouseClick_Impl();

public:
	static bool OnMouseClick();
	static void CreateButton(PanelRect panel, std::string text, std::function<void()> function);
	static void CreateExamplePanel(PanelRect panel, std::string string);

	template<class T>
	static void CreateVariableTracker(PanelRect panel, T& reference, std::string additionalText);

	static void Update(float DeltaTime);
	static void Render(SDL_Renderer& renderer);
};

template<class T>
inline void UI::CreateVariableTracker(PanelRect panel, T& reference, std::string additionalText)
{
	Get()->CreateVariableTracker_Impl(panel, reference, additionalText);
}

template<class T>
inline void UI::CreateVariableTracker_Impl(PanelRect panel, T& reference, std::string additionalText)
{
	UI_VariableTracker<T>* tracker = new UI_VariableTracker<T>(panel, reference, additionalText);
	AddUIElementToScreenMap(tracker, panel);
}
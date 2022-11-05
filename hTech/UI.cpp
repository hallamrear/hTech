#include "pch.h"
#include "UI.h"
#include "UI_Button.h"
#include "InputManager.h"
#include "UI_VariableTracker.h"
#include "UI_TextPanel.h"

UI* UI::mInstance = nullptr;

UI::UI() : mWindowDimensions(Settings::Get()->GetWindowDimensions())
{
	mUIGridColumnCount = (int)mWindowDimensions.X / UI_TILE_SIZE;
	mUIGridRowCount = (int)mWindowDimensions.Y / UI_TILE_SIZE;

	mUIMap = new UI_Element * *[mUIGridColumnCount];
	for (size_t i = 0; i < mUIGridColumnCount; i++)
	{
		mUIMap[i] = new UI_Element* [mUIGridRowCount];
		for (size_t j = 0; j < mUIGridRowCount; j++)
		{
			mUIMap[i][j] = nullptr;
		}
	}

	mFocusedObject = nullptr;
}

UI::~UI()
{
	for (size_t i = 0; i < mUIGridColumnCount; i++)
	{
		for (size_t j = 0; j < mUIGridRowCount; j++)
		{
			mUIMap[i][j] = nullptr;
		}

		delete[] mUIMap[i];
		mUIMap[i] = nullptr;
	}

	delete[] mUIMap;
	mUIMap = nullptr;
}

UI* UI::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new UI();
	}

	return mInstance;
}

bool UI::OnMouseClick()
{
	return Get()->OnMouseClick_Impl();
}

void UI::CreateButton_Impl(UI_Panel panel, std::string text, std::function<void()> function)
{
	AddUIElementToScreenMap(new UI_Button(panel, text, function), panel);
}

void UI::AddUIElementToScreenMap(UI_Element* element, UI_Panel panel)
{
	if (element != nullptr)
	{
		for (size_t x = panel.X; x != (panel.X + panel.W); x++)
		{
			for (size_t y = panel.Y; y != (panel.Y + panel.H); y++)
			{
				mUIMap[x][y] = element;
			}
		}

		mUIElements.push_back(element);
	}
}

void UI::CreateButton(UI_Panel panel, std::string text, std::function<void()> function)
{
	Get()->CreateButton_Impl(panel, text, function);
}

void UI::CreateExamplePanel(UI_Panel panel, std::string string)
{
	Get()->CreateExamplePanel_Impl(panel, string);
}

void UI::Update_Impl(float DeltaTime)
{
	for(const auto& itr : mUIElements)
	{
		itr->Update(DeltaTime);
	}
}

void UI::Render_Impl(SDL_Renderer& renderer)
{
	mUIGridLayoutPoints.clear();
	Vector2 dim = Settings::Get()->GetWindowDimensions();
	Vector2 pos;
	int count = 0;

	for (int i = 0; i < dim.X; i += UI_TILE_SIZE)
	{
		for (int j = 0; j < dim.Y; j += UI_TILE_SIZE)
		{
			SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
			SDL_RenderDrawPoint(&renderer, i, j);
			mUIGridLayoutPoints.push_back(SDL_Point{ i, j });
		}
	}

	SDL_RenderDrawPoints(&renderer, mUIGridLayoutPoints.data(), mUIGridLayoutPoints.size());

	for (const auto& itr : mUIElements)
	{
		itr->Render(renderer);
	}
}

void UI::CreateExamplePanel_Impl(UI_Panel panel, std::string string)
{
	AddUIElementToScreenMap(new UI_TextPanel(panel, string), panel);
}

bool UI::OnMouseClick_Impl()
{
	Vector2 mousePos = InputManager::Get()->GetMouseScreenPosition();
	unsigned int clickPosX = (int)mousePos.X / UI_TILE_SIZE;
	unsigned int clickPosY = (int)mousePos.Y / UI_TILE_SIZE;

	if (mUIMap[clickPosX][clickPosY] != nullptr)
	{
		if (mFocusedObject != nullptr)
		{
			mFocusedObject->SetInFocus(false);
		}

		mFocusedObject = mUIMap[clickPosX][clickPosY];
		mFocusedObject->SetInFocus(true);
		mFocusedObject->OnClick();
		return true;
	}

	return false;
}

void UI::Update(float DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

void UI::Render(SDL_Renderer& renderer)
{
	Get()->Render_Impl(renderer);
}
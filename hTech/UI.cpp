#include "pch.h"
#include "UI.h"
#include "IRenderer.h"

#ifdef DEFINE_OLD_UI
#include "UI_Button.h"
#include "InputManager.h"
#include "UI_VariableTracker.h"
#include "UI_TextPanel.h"
#include "Console.h"

UI* UI::mInstance = nullptr;

UI::UI()
{
	mUIGridColumnCount = Console::Query("WindowDimensionsW") / UI_TILE_SIZE;
	mUIGridRowCount    = Console::Query("WindowDimensionsH") / UI_TILE_SIZE;

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

UI_Button* UI::CreateButton_Impl(UI_Panel panel, std::string text, std::function<void()> function)
{
	UI_Button* button = new UI_Button(panel, text, function);
	AddUIElementToScreenMap(button, panel);
	return button;
}

void UI::RebuildUIGrid()
{
	mUIGridLayoutPoints.clear();
	Vector2 dim;
	dim.X = Console::Query("WindowDimensionsW");
	dim.Y = Console::Query("WindowDimensionsH");
	Vector2 pos;
	int count = 0;

	for (int i = 0; i < dim.X; i += UI_TILE_SIZE)
	{
		for (int j = 0; j < dim.Y; j += UI_TILE_SIZE)
		{
			mUIGridLayoutPoints.push_back(SDL_Point{ i, j });
		}
	}

	mStoredWindowDimensions = dim;
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

UI_Button* UI::CreateButton(UI_Panel panel, std::string text, std::function<void()> function)
{
	return Get()->CreateButton_Impl(panel, text, function);
}

UI_TextPanel* UI::CreateTextPanel(UI_Panel panel, std::string string)
{
	return Get()->CreateTextPanel_Impl(panel, string);
}

void UI::Update_Impl(float DeltaTime)
{
	Vector2 dimensions;
	dimensions.X = Console::Query("WindowDimensionsW");
	dimensions.Y = Console::Query("WindowDimensionsH");

	if (dimensions != mStoredWindowDimensions)
	{
		RebuildUIGrid();
	}

	for(const auto& itr : mUIElements)
	{
		itr->Update(DeltaTime);
	}
}

void UI::Render_Impl(IRenderer& renderer)
{
	SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
	SDL_RenderDrawPoints(&renderer, mUIGridLayoutPoints.data(), mUIGridLayoutPoints.size());

	for (const auto& itr : mUIElements)
	{
		itr->Render(renderer);
	}
}

UI_TextPanel* UI::CreateTextPanel_Impl(UI_Panel panel, std::string string)
{
	UI_TextPanel* text = new UI_TextPanel(panel, string);
	AddUIElementToScreenMap(text, panel);
	return text;
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

void UI::Render(IRenderer& renderer)
{
	Get()->Render_Impl(renderer);
}
#endif

UI* UI::mInstance = nullptr;

UI::UI()
{

}

UI::~UI()
{

}

UI* UI::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new UI();
	}

	return mInstance;
}

void UI::Update(float DeltaTime)
{
	Get()->Update_Impl(DeltaTime);
}

void UI::Render(IRenderer& renderer)
{
	Get()->Render_Impl(renderer);
}

void UI::Update_Impl(float DeltaTime)
{

}

void UI::Render_Impl(IRenderer& renderer)
{

}

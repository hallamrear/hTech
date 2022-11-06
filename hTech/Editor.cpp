#include "pch.h"
#include "Editor.h"
#include "UI.h"
#include "InputManager.h"
#include "TextureCache.h"
#include "Texture.h"
#include <functional>

Editor* Editor::mInstance = nullptr;

Editor::Editor() : mMouseScreenPosition(InputManager::Get()->GetMouseScreenPosition()), mMouseWorldPosition(InputManager::Get()->GetMouseWorldPosition())
{
    mCursorTextures[0] = TextureCache::GetTexture("Assets/Cursor_Move.png");
    mCursorTextures[1] = TextureCache::GetTexture("Assets/Cursor_Inspect.png");
    mCursorTextures[2] = TextureCache::GetTexture("Assets/Cursor_Rotate.png");

    //Setting up editor controls!
    UI::CreateButton(UI_Panel(1, 1, 1, 1), "†", std::bind(&Editor::SetCursorStateToNoMode, this));
    UI::CreateButton(UI_Panel(2, 1, 1, 1), "M", std::bind(&Editor::SetCursorStateToMoveMode, this));
    UI::CreateButton(UI_Panel(3, 1, 1, 1), "R", std::bind(&Editor::SetCursorStateToRotateMode, this));
    UI::CreateButton(UI_Panel(4, 1, 1, 1), "I", std::bind(&Editor::SetCursorStateToInspectMode, this));

    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_0, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToNoMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_1, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToMoveMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_2, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToRotateMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_3, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToInspectMode, this));

    mCurrentCursorState = EDITOR_STATE::NONE;
    mSelectedEntity = nullptr;
}

Editor* Editor::Get()
{
    if (mInstance == nullptr)
        mInstance = new Editor();

    return mInstance;
}

Editor::~Editor()
{

}

void Editor::Update(float deltaTime)
{
    Get()->Update_Impl(deltaTime);
}

void Editor::Update_Impl(float deltaTime)
{
    switch (mCurrentCursorState)
    {
    case EDITOR_STATE::MOVE:
        break;
    case EDITOR_STATE::INSPECT:
        break;
    case EDITOR_STATE::ROTATE:
        break;
    default:
    case EDITOR_STATE::NONE:
        break;
    }
}

void Editor::Render(SDL_Renderer& renderer)
{
    Get()->Render_Impl(renderer);
}

void Editor::Render_Impl(SDL_Renderer& renderer)
{
    if (mCurrentCursorState != EDITOR_STATE::NONE)
    {
        if (mCursorTextures[(int)mCurrentCursorState] != nullptr)
        {
            mCursorTextures[(int)mCurrentCursorState]->Render(renderer, InputManager::Get()->GetMouseWorldPosition(), 0.0f);
        }
    }
}

void Editor::OnMouseClick()
{
    Get()->OnMouseClick_Impl();
}

void Editor::OnMouseClick_Impl()
{

}

void Editor::SetEditorCursorState(EDITOR_STATE state)
{
    Get()->SetEditorCursorState_Impl(state);
}

void Editor::SetEditorCursorState_Impl(EDITOR_STATE state)
{
    mCurrentCursorState = state;
}

void Editor::SetCursorStateToMoveMode()
{
    mCurrentCursorState = EDITOR_STATE::MOVE;
}

void Editor::SetCursorStateToRotateMode()
{
    mCurrentCursorState = EDITOR_STATE::ROTATE;
}

void Editor::SetCursorStateToInspectMode()
{
    mCurrentCursorState = EDITOR_STATE::INSPECT;
}

void Editor::SetCursorStateToNoMode()
{
    mCurrentCursorState = EDITOR_STATE::NONE;
}

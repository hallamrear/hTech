#include "pch.h"
#include "Editor.h"
#include "UI.h"
#include "InputManager.h"
#include "TextureCache.h"
#include "Texture.h"
#include "World.h"
#include "UI_Button.h"
#include "Camera.h"
#include <functional>

#include "World.h"
#include "UI_Button.h"
#include "Time.h"

Editor* Editor::mInstance = nullptr;
UI_Button* button = nullptr;

Editor::Editor() : mMouseScreenPosition(InputManager::Get()->GetMouseScreenPosition()), mMouseWorldPosition(InputManager::Get()->GetMouseWorldPosition())
{
    mCursorTextures[0] = TextureCache::GetTexture("Assets/Cursor_Select.png");
    mCursorTextures[1] = TextureCache::GetTexture("Assets/Cursor_Move.png");
    mCursorTextures[2] = TextureCache::GetTexture("Assets/Cursor_Inspect.png");
    mCursorTextures[3] = TextureCache::GetTexture("Assets/Cursor_Rotate.png");
    mSelectedEntities = std::vector<Entity*>();

    //Setting up editor controls!
    button = UI::CreateButton(UI_Panel(1, 1, 1, 1), "#", std::bind(&Editor::SetCursorStateToNoMode, this));
    UI::CreateButton(UI_Panel(2, 1, 1, 1), "M", std::bind(&Editor::SetCursorStateToMoveMode, this));
    UI::CreateButton(UI_Panel(3, 1, 1, 1), "R", std::bind(&Editor::SetCursorStateToRotateMode, this));
    UI::CreateButton(UI_Panel(4, 1, 1, 1), "I", std::bind(&Editor::SetCursorStateToInspectMode, this));

    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_0, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToNoMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_1, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToSelectMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_2, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToMoveMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_3, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToRotateMode, this));
    InputManager::Get()->Bind(IM_KEY_CODE::IM_KEY_4, IM_KEY_STATE::IM_KEY_PRESSED, std::bind(&Editor::SetCursorStateToInspectMode, this));

    mCurrentCursorState = EDITOR_STATE::NONE;

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED,  std::bind(&Editor::MousePress, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_HELD,     std::bind(&Editor::MouseHold, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_RELEASED, std::bind(&Editor::MouseRelease, this));
}

Entity* selected = nullptr;

void Editor::MousePress()
{
    mDragStartWS = InputManager::Get()->GetMouseWorldPosition();

    switch (mCurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
        mIsDraggingRect = true;
        break;
    case EDITOR_STATE::MOVE:
    {
        mMovementMousePosLastFrame = InputManager::Get()->GetMouseWorldPosition();

        if (mSelectedEntities.size() <= 0)
        {
            selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
        }
    }
        break;
    case EDITOR_STATE::INSPECT:
    {
        selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
    }
        break;
    case EDITOR_STATE::ROTATE:
    {
        selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
    }
        break;
    case EDITOR_STATE::NONE:
    {

    }
        break;
    default:
        break;
    }

}

void Editor::MouseHold()
{
    mDragCurrentWS = InputManager::Get()->GetMouseWorldPosition();

    switch (mCurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
    {
        selectionRect = WorldRectangle(mDragStartWS, mDragCurrentWS);
        mIsDraggingRect = true;
    }
        break;
    case EDITOR_STATE::MOVE:
    {
        mDragDifferenceThisFrame = (mDragCurrentWS - mMovementMousePosLastFrame);

        if (mSelectedEntities.size() > 0)
        {
            for (size_t i = 0; i < mSelectedEntities.size(); i++)
            {
                mSelectedEntities[i]->GetTransform().Position += mDragDifferenceThisFrame;
            }
        }
        else
        {
            if (selected)
            {
                selected->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
            }
        }

        mMovementMousePosLastFrame = mDragCurrentWS;
    }
        break;
    case EDITOR_STATE::INSPECT:
    {

    }
        break;
    case EDITOR_STATE::ROTATE:
    {
        if (selected)
        {
            mDragDifferenceThisFrame = (mDragCurrentWS - mMovementMousePosLastFrame);
            selected->GetTransform().Rotate(mDragDifferenceThisFrame.GetMagnitude() * Time::DeltaTime());
        }
    }
        break;
    case EDITOR_STATE::NONE:
    {

    }
        break;
    default:
        break;
    }
}

void Editor::MouseRelease()
{
    mDragEndWS = InputManager::Get()->GetMouseWorldPosition();

    switch (mCurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
        selectionRect = WorldRectangle(mDragStartWS, mDragEndWS);
        World::QuerySpaceForEntities(selectionRect, mSelectedEntities);
        break;
    case EDITOR_STATE::MOVE:
        mSelectedEntities.clear();
        selected = nullptr;
        break;
    case EDITOR_STATE::INSPECT:
        break;
    case EDITOR_STATE::ROTATE:
        mSelectedEntities.clear();
        selected = nullptr;
        break;
    case EDITOR_STATE::NONE:
        break;
    default:
        break;
    }

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
    button->GetPanel().BackgroundColour.R++;
    button->GetPanel().BackgroundColour.R = button->GetPanel().BackgroundColour.R % 255;

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

    for (size_t i = 0; i < mSelectedEntities.size(); i++)
    {
        Vector2 rectTR = Camera::WorldToScreen(mSelectedEntities[i]->GetTransform().Position + Vector2(-32, 32));
        SDL_Rect rect{};
        rect.x = (int)rectTR.X;
        rect.y = (int)rectTR.Y;
        rect.w = 64;
        rect.h = 64;
        SDL_RenderDrawRect(&renderer, &rect);
    }

    if (mIsDraggingRect)
    {
        Vector2 points[4] =
        {
            {(float)selectionRect.X,				   (float)selectionRect.Y},
            {(float)selectionRect.X + selectionRect.W, (float)selectionRect.Y},
            {(float)selectionRect.X,				   (float)selectionRect.Y - selectionRect.H},
            {(float)selectionRect.X + selectionRect.W, (float)selectionRect.Y - selectionRect.H}
        };

        Vector2 r;
        SDL_Rect c{};
        for (size_t i = 0; i < 4; i++)
        {
            r = Camera::WorldToScreen(points[i]);
            c.w = 4;
            c.h = 4;
            c.x = (int)r.X - (c.w / 2);
            c.y = (int)r.Y - (c.h / 2);
            SDL_RenderFillRect(&renderer, &c);
        }

        SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
        selectionRect.Render(renderer);
    }

    if (selected)
    {
        SDL_Rect d{};
        Vector2 position = Camera::WorldToScreen(selected->GetTransform().Position + Vector2(-32.0f, 32.0f));
        d.x = (int)position.X;
        d.y = (int)position.Y;
        d.w = 64;
        d.h = 64;
        SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(&renderer, &d);
    }
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
    SDL_ShowCursor(false);
}

void Editor::SetCursorStateToRotateMode()
{
    mCurrentCursorState = EDITOR_STATE::ROTATE;
    SDL_ShowCursor(false);
}

void Editor::SetCursorStateToInspectMode()
{
    mCurrentCursorState = EDITOR_STATE::INSPECT;
    SDL_ShowCursor(false);
}

void Editor::SetCursorStateToNoMode()
{
    mCurrentCursorState = EDITOR_STATE::NONE;
    SDL_ShowCursor(true);
}

void Editor::SetCursorStateToSelectMode()
{
    mCurrentCursorState = EDITOR_STATE::SELECT;
    SDL_ShowCursor(true);
    selected = nullptr;
}
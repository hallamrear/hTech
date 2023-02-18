#include "pch.h"
#include "Editor.h"

#if _DEBUG

#include "UI.h"
#include "InputManager.h"
#include "TextureCache.h"
#include "Texture.h"
#include "World.h"
#include "UI_Button.h"
#include "Camera.h"
#include <functional>
#include "Entity.h"
#include "World.h"
#include "UI_Button.h"
#include "Time.h"

Editor* Editor::m_Instance = nullptr;

Editor::Editor() : m_MouseScreenPositionRef(InputManager::Get()->GetMouseScreenPosition()), m_MouseWorldPositionRef(InputManager::Get()->GetMouseWorldPosition())
{
    m_SelectedEntities = std::vector<Entity*>();

    //Setting up editor controls!
    m_CurrentCursorState = EDITOR_STATE::NONE;

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED,  std::bind(&Editor::MousePress, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_HELD,     std::bind(&Editor::MouseHold, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_RELEASED, std::bind(&Editor::MouseRelease, this));
}

Entity* selected = nullptr;

void Editor::MousePress()
{
    m_DragStartWS = InputManager::Get()->GetMouseWorldPosition();

    switch (m_CurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
        m_IsDraggingRect = true;
        break;
    case EDITOR_STATE::MOVE:
    {
        m_MousePosPreviousFrame = InputManager::Get()->GetMouseWorldPosition();

        if (m_SelectedEntities.size() <= 0)
        {
            selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
        }
    }
        break;
    case EDITOR_STATE::INSPECT:
    {
        Entity* found = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());

        if (found != nullptr)
        {
            selected = found;
        }
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
    m_DragCurrentWS = InputManager::Get()->GetMouseWorldPosition();

    switch (m_CurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
    {
        m_SelectionRect = WorldRectangle(m_DragStartWS, m_DragCurrentWS);
        m_IsDraggingRect = true;
    }
        break;
    case EDITOR_STATE::MOVE:
    {
        m_DragDeltaThisFrame = (m_DragCurrentWS - m_MousePosPreviousFrame);

        if (m_SelectedEntities.size() > 0)
        {
            for (size_t i = 0; i < m_SelectedEntities.size(); i++)
            {
                m_SelectedEntities[i]->GetTransform().Position += m_DragDeltaThisFrame;
            }
        }
        else
        {
            if (selected)
            {
                selected->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
            }
        }

        m_MousePosPreviousFrame = m_DragCurrentWS;
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
            Vector2 diff = (m_DragStartWS - m_DragCurrentWS);
            selected->GetTransform().Rotate(diff.X * Time::DeltaTime());
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
    m_DragEndWS = InputManager::Get()->GetMouseWorldPosition();

    switch (m_CurrentCursorState)
    {
    case EDITOR_STATE::SELECT:
        m_SelectionRect = WorldRectangle(m_DragStartWS, m_DragEndWS);
        World::QuerySpaceForEntities(m_SelectionRect, m_SelectedEntities);
        break;
    case EDITOR_STATE::MOVE:
        break;
    case EDITOR_STATE::INSPECT:
        break;
    case EDITOR_STATE::ROTATE:
        m_SelectedEntities.clear();
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
    if (m_Instance == nullptr)
        m_Instance = new Editor();

    return m_Instance;
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
    switch (m_CurrentCursorState)
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
    Camera::RenderProperties();

    if (ImGui::Begin("Properties", 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (selected)
        {
            selected->RenderProperties();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Assets", 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        TextureCache::RenderProperties();
    }
    ImGui::End();

    for (size_t i = 0; i < m_SelectedEntities.size(); i++)
    {
        Vector2 rectTR = Camera::WorldToScreen(m_SelectedEntities[i]->GetTransform().Position + Vector2(-32, 32));
        SDL_Rect rect{};
        rect.x = (int)rectTR.X;
        rect.y = (int)rectTR.Y;
        rect.w = 64;
        rect.h = 64;
        SDL_RenderDrawRect(&renderer, &rect);
    }

    if (m_IsDraggingRect)
    {
        Vector2 points[4] =
        {
            {(float)m_SelectionRect.X,				   (float)m_SelectionRect.Y},
            {(float)m_SelectionRect.X + m_SelectionRect.W, (float)m_SelectionRect.Y},
            {(float)m_SelectionRect.X,				   (float)m_SelectionRect.Y - m_SelectionRect.H},
            {(float)m_SelectionRect.X + m_SelectionRect.W, (float)m_SelectionRect.Y - m_SelectionRect.H}
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
        m_SelectionRect.Render(renderer);
    }

    if (selected)
    {
        SDL_Rect d{};
        d.w = 256;
        d.h = 256;
        Vector2 Position = Camera::WorldToScreen(selected->GetTransform().Position + Vector2(0 - (d.w / 2), d.h / 2));
        d.x = (int)Position.X;
        d.y = (int)Position.Y;
        SDL_SetRenderDrawColor(&renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(&renderer, &d);
        SDL_RenderDrawLine(&renderer, d.x, d.y, d.x + d.w, d.y + d.h);
        SDL_RenderDrawLine(&renderer, d.x + d.w, d.y, d.x, d.y + d.h);
    }
}

void Editor::SetEditorCursorState(EDITOR_STATE state)
{
    Get()->SetEditorCursorState_Impl(state);
}

EDITOR_STATE Editor::GetEditorCursorState()
{
    return Get()->m_CurrentCursorState;
}

void Editor::ClearSelected()
{
    return Get()->ClearSelected_Impl();
}

void Editor::ClearSelected_Impl()
{
    selected = nullptr;
    m_SelectedEntities.clear();
}

void Editor::SetEditorCursorState_Impl(EDITOR_STATE state)
{
    m_CurrentCursorState = state;
}

void Editor::SetCursorStateToMoveMode()
{
    m_CurrentCursorState = EDITOR_STATE::MOVE;
}

void Editor::SetCursorStateToRotateMode()
{
    m_CurrentCursorState = EDITOR_STATE::ROTATE;
}

void Editor::SetCursorStateToInspectMode()
{
    m_CurrentCursorState = EDITOR_STATE::INSPECT;
}

void Editor::SetCursorStateToNoMode()
{
    m_CurrentCursorState = EDITOR_STATE::NONE;
}

void Editor::SetCursorStateToSelectMode()
{
    m_CurrentCursorState = EDITOR_STATE::SELECT;
    selected = nullptr;
}

void Editor::SetSelectedEntity_Impl(Entity* entity)
{
    ClearSelected_Impl();
    selected = entity;
}

void Editor::SetSelectedEntity(Entity* entity)
{
    Get()->SetSelectedEntity_Impl(entity);
}

Entity* Editor::GetSelectedEntity_Impl()
{
    return selected;
}

std::vector<Entity*> Editor::GetSelectedEntities_Impl()
{
    return m_SelectedEntities;
}

Entity* Editor::GetSelectedEntity()
{
    return Get()->GetSelectedEntity_Impl();
}

std::vector<Entity*> Editor::GetSelectedEntities()
{
    return Get()->GetSelectedEntities_Impl();
}

#endif
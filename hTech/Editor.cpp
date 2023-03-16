#include "pch.h"
#include "Editor.h"
#include "ImGuiIncludes.h"
#include "InputManager.h"
#include "World.h"
#include "ProjectLoader.h"
#include "Engine.h"
#include "Time.h"
#include "Camera.h"
#include "TextureCache.h"

///Includes specifically for the file opening screen.
#include <ShlObj_core.h>
#include <SDL_syswm.h>

///todo : clean up
///Needed for casts and should be removed later.
#include "OriginalRenderer.h"
#include "OriginalWindow.h"
#include "Console.h"

Editor* Editor::m_Instance = nullptr;

Editor::Editor() : m_MouseScreenPositionRef(InputManager::Get()->GetMouseScreenPosition()), m_MouseWorldPositionRef(InputManager::Get()->GetMouseWorldPosition())
{
    m_SelectedEntities = std::vector<Entity*>();

    //Setting up editor controls!
    m_CurrentCursorState = EDITOR_STATE::NONE;

    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_PRESSED,  std::bind(&Editor::MousePress, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_HELD,     std::bind(&Editor::MouseHold, this));
    InputManager::Bind(IM_MOUSE_CODE::IM_MOUSE_LEFT_CLICK, IM_KEY_STATE::IM_KEY_RELEASED, std::bind(&Editor::MouseRelease, this));

	m_AutosaveEnabled = true;
	m_AutosaveTimer = 0.0f;
	m_AutosaveCooldown = 15.0f;
}

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
            m_Selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
        }
    }
        break;
    case EDITOR_STATE::INSPECT:
    {
        Entity* found = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());

        if (found != nullptr)
        {
            m_Selected = found;
        }
    }
        break;
    case EDITOR_STATE::ROTATE:
    {
        m_Selected = World::FindNearestEntityToPosition(InputManager::Get()->GetMouseWorldPosition());
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
            if (m_Selected)
            {
                m_Selected->GetTransform().Position = InputManager::Get()->GetMouseWorldPosition();
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
        if (m_Selected)
        {
            Vector2 diff = (m_DragStartWS - m_DragCurrentWS);
            m_Selected->GetTransform().Rotate(diff.X * Time::DeltaTime());
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
        m_Selected = nullptr;
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
	if (ProjectLoader::HasProjectLoaded() && Engine::GetGameState() != GAME_STATE::RUNNING)
	{
		if (m_AutosaveEnabled)
		{
			m_AutosaveTimer += deltaTime;

			if (m_AutosaveTimer >= m_AutosaveCooldown)
			{
				ProjectLoader::SaveProject();
				m_AutosaveTimer = 0.0f;
			}
		}
	}

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

void Editor::Render(IRenderer& renderer)
{
    Get()->Render_Impl(renderer);
}

void Editor::Render_Impl(IRenderer& renderer)
{
    Camera::RenderProperties();

    if (ImGui::Begin("Properties", 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (m_Selected)
        {
            m_Selected->RenderProperties();
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
        Vector2 pos = Camera::WorldToScreen(m_SelectedEntities[i]->GetTransform().Position);
        WorldRectangle rect = WorldRectangle(pos.X, pos.Y, 64, 64);
        renderer.Render_ScreenSpaceRectangle(rect, false);
    }

    if (m_IsDraggingRect)
    {
        int halfW = m_SelectionRect.W / 2;
        int halfH = m_SelectionRect.H / 2;

        Vector2 points[4] =
        {
            {(float)m_SelectionRect.X,				       (float)m_SelectionRect.Y},
            {(float)m_SelectionRect.X + m_SelectionRect.W, (float)m_SelectionRect.Y},
            {(float)m_SelectionRect.X,				       (float)m_SelectionRect.Y - m_SelectionRect.H},
            {(float)m_SelectionRect.X + m_SelectionRect.W, (float)m_SelectionRect.Y - m_SelectionRect.H}
        };

        Vector2 r;
        WorldRectangle corner = WorldRectangle(0, 0, 0, 0);
        Vector2 p;
        for (size_t i = 0; i < 4; i++)
        {
            p = Camera::WorldToScreen(points[i]);

            corner.W = 4;
            corner.H = 4;
            corner.X = p.X;
            corner.Y = p.Y;
            renderer.Render_ScreenSpaceRectangle(corner);
        }

        renderer.SetPrimativeDrawColour(Colour::White);
        WorldRectangle outline = m_SelectionRect;
        outline.X += (m_SelectionRect.W / 2);
        outline.Y -= (m_SelectionRect.H / 2);
        renderer.Render_WorldSpaceRectangle(outline, false);
    }

    if (m_Selected)
    {
        WorldRectangle selectedRect = WorldRectangle(0, 0, 256, 256);
        Vector2 Position = Camera::WorldToScreen(m_Selected->GetTransform().Position);
        selectedRect.X = (int)Position.X;
        selectedRect.Y = (int)Position.Y;

        renderer.SetPrimativeDrawColour(Colour(255, 255, 0, 255));
        renderer.Render_ScreenSpaceRectangle(selectedRect, false);

        int halfWidth = selectedRect.W / 2;
        int halfHeight = selectedRect.H / 2;
        renderer.Render_ScreenSpaceLine(
            Vector2(selectedRect.X - halfWidth, selectedRect.Y - halfHeight), 
            Vector2(selectedRect.X + halfWidth, selectedRect.Y + halfHeight));

        renderer.Render_ScreenSpaceLine(
            Vector2(selectedRect.X + halfWidth, selectedRect.Y - halfHeight),
            Vector2(selectedRect.X - halfWidth, selectedRect.Y + halfHeight));
    }
      
    static bool showNewProjectModal = false,
		showOpenProjectModal = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Project"))
			{
				static std::string projectName;
				showNewProjectModal = true;
			}

			if (ImGui::MenuItem("Open Project"))
			{
				std::string projectFilePath = "";
				if (OpenProject(projectFilePath))
				{
					ProjectLoader::LoadProject(projectFilePath);
					m_AutosaveTimer = 0.0f;
				}
			}

			if (ImGui::MenuItem("Save Project"))
			{
				ProjectLoader::SaveProject();
				m_AutosaveTimer = 0.0f;
			}

			if (ImGui::MenuItem("Close Project"))
			{
				ProjectLoader::UnloadProject();
				m_AutosaveTimer = 0.0f;
			}

			if (ImGui::BeginMenu("Exit##Menu"))
			{
				if (ImGui::MenuItem("Exit with Saving"))
				{
					ProjectLoader::SaveProject();
					Engine::SetIsRunning(false);
				}

				ImGui::Dummy(Vector2(-FLT_MAX, 100.0f));

				if (ImGui::MenuItem("Exit without Saving"))
				{
					Engine::SetIsRunning(false);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::Text(ProjectLoader::GetLoadedProjectName().c_str());

		if (ImGui::BeginMenu("Options"))
		{
			std::string autosaveStr;
			m_AutosaveEnabled == true ? autosaveStr = "Autosave Enabled" : autosaveStr = "Autosave Disabled";

			if (ImGui::MenuItem(autosaveStr.c_str()))
			{
				m_AutosaveEnabled = !m_AutosaveEnabled;
			}

			bool queryHash = (bool)Console::Query("DrawHashMap");
			if (ImGui::MenuItem("Spatial Hash"))
			{
				if (queryHash)
				{
					Console::Run("DrawHashMap 0");
				}
				else
				{
					Console::Run("DrawHashMap 1");
				}
			}

			bool queryC = (bool)Console::Query("DrawColliders");
			if (ImGui::MenuItem("Collider Outlines"))
			{
				if (queryC)
				{
					Console::Run("DrawColliders 0");
				}
				else
				{
					Console::Run("DrawColliders 1");
				}
			}
			ImGui::EndMenu();
		}

		if (m_AutosaveEnabled)
		{
			std::string timeToSave = "Autosave in: " + std::to_string(m_AutosaveCooldown - m_AutosaveTimer) + " seconds";
			ImGui::MenuItem(timeToSave.c_str());
		}

		ImGui::EndMainMenuBar();

		if (showNewProjectModal)
		{
			if (ImGui::IsPopupOpen("New Project") == false)
				ImGui::OpenPopup("New Project");

			// Always center this window when appearing
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("New Project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				static std::string projectName = "";
				ImGui::Text("Create a new project?\n");
				ImGui::Text("Project Name: "); ImGui::SameLine(); ImGui::InputText("##ProjectNameInput", &projectName);
				ImGui::Separator();

				if (ImGui::Button("Create", ImVec2(120, 0)))
				{
					ProjectLoader::CreateProject(projectName);
					projectName = "";
					showNewProjectModal = false;
					m_AutosaveTimer = 0.0f;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0)))
				{
					showNewProjectModal = false;
					ImGui::CloseCurrentPopup();
					projectName = "";
				}

				ImGui::EndPopup();
			}
		}
	}


	ImGui::Begin("Render Window", 0, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	static bool showDeletionConfirmation = false;
	ImGui::BeginMenuBar();
	{
		switch (Engine::GetGameState())
		{
		case GAME_STATE::RUNNING:
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			if (ImGui::Button("Stop"))
			{
				Engine::SetGameState(GAME_STATE::STOPPED);
				World::ResetWorldEntities();
			}
			ImGui::PopStyleColor();
		}
		break;

		case GAME_STATE::PAUSED:
		{

		}
		break;

		case GAME_STATE::STOPPED:
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
			if (ImGui::Button("Start"))
			{
				Engine::SetGameState(GAME_STATE::RUNNING);
				World::ResetWorldEntities();
				World::CallStartFunctionOnAllEntites();
				Editor::ClearSelected();
			}
			ImGui::PopStyleColor();

			if (ImGui::Button("Create Empty entity"))
			{
				World::CreateEntity();
			}

			if (ImGui::Button("Delete selected entity"))
			{
				if (Editor::GetSelectedEntity() != nullptr)
				{
					showDeletionConfirmation = true;
				}
			}

			ImGui::SameLine();

			//IMPLEMENT Editor tools window
			std::string modeStr = "";
			EDITOR_STATE state = Editor::GetEditorCursorState();

			float buttonSize = 16;

			if (ImGui::Button("M##Move", Vector2(buttonSize, buttonSize)))
			{
				Editor::SetEditorCursorState(EDITOR_STATE::MOVE);
			}

			ImGui::SameLine();

			if (ImGui::Button("R##Rotate", Vector2(buttonSize, buttonSize)))
			{
				Editor::SetEditorCursorState(EDITOR_STATE::ROTATE);
			}

			ImGui::SameLine();

			if (ImGui::Button("I##Inspect", Vector2(buttonSize, buttonSize)))
			{
				Editor::SetEditorCursorState(EDITOR_STATE::INSPECT);
			}

			ImGui::SameLine();

			if (ImGui::Button("S##Select", Vector2(buttonSize, buttonSize)))
			{
				Editor::SetEditorCursorState(EDITOR_STATE::SELECT);
			}

			switch (state)
			{
			case EDITOR_STATE::SELECT:
				modeStr = "Selection";
				break;
			case EDITOR_STATE::MOVE:
				modeStr = "Move";
				break;
			case EDITOR_STATE::INSPECT:
				modeStr = "Inspect";
				break;
			case EDITOR_STATE::ROTATE:
				modeStr = "Rotate";
				break;
			case EDITOR_STATE::NONE:
			default:
				modeStr = "No mode";
				break;
			}
			ImGui::SameLine(0.0f, buttonSize);
			ImGui::Text("Current mode: %s", modeStr.c_str());

		}
		break;

		default:
			break;
		}
	}

	ImGui::EndMenuBar();


	if (showDeletionConfirmation)
	{
		if (ImGui::IsPopupOpen("Are you sure?") == false)
			ImGui::OpenPopup("Are you sure?");

		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			std::string text = "Are you sure you want to delete entity \"" + Editor::GetSelectedEntity()->GetName() + "\"";
			ImGui::Text(text.c_str());

			if (ImGui::Button("Delete", ImVec2(120, 0)))
			{
				World::DestroyEntity(Editor::GetSelectedEntity());
				Editor::ClearSelected();
				showDeletionConfirmation = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				showDeletionConfirmation = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	
	//todo : this needs sorting out later on.
	OriginalRenderer& castedRenderer = (OriginalRenderer&)renderer;

	SDL_SetRenderTarget(castedRenderer.GetAPIRenderer(), NULL);
	Vector2 size;
	size.X = ImGui::GetWindowWidth();
	size.Y = ImGui::GetWindowHeight();
	Vector2 pos;
	pos.X = ImGui::GetWindowPos().x;
	pos.Y = ImGui::GetWindowPos().y;
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	SDL_Rect renderDstQuad = { (int)(pos.X + vMin.x), (int)(pos.Y + vMin.y), (int)(vMax.x - vMin.x), (int)(vMax.y - vMin.y) };
	SDL_Rect renderSrcQuad = renderDstQuad;
	renderSrcQuad.w /= (int)Camera::ZoomLevel;
	renderSrcQuad.h /= (int)Camera::ZoomLevel;

	SDL_RenderSetClipRect(castedRenderer.GetAPIRenderer(), &renderDstQuad);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	//Render to screen
	SDL_RenderCopyEx(castedRenderer.GetAPIRenderer(), castedRenderer.GetRenderTexture(), &renderSrcQuad, &renderDstQuad, 0.0F, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

// callback function
INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
	return 0;
}

bool Editor::OpenProject(std::string& path)
{
	//todo : sortout
	//This is not abstracted :(
	SDL_SysWMinfo wmInfo{};
	SDL_VERSION(&wmInfo.version);
	OriginalWindow* window = dynamic_cast<OriginalWindow*>(&Engine::GetWindow());
	if (window == nullptr)
		return false;

	SDL_GetWindowWMInfo(window->GetAPIWindow(), &wmInfo);

	// common dialog box structure, setting all fields to 0 is important
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = wmInfo.info.win.window;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("Project Files\0*.hProj\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		// use ofn.lpstrFile here
		path = ofn.lpstrFile;
		return true;
	}

	path = "";
	return false;
}
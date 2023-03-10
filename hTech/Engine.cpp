#include "pch.h"
#include "Engine.h"
#include "Console.h"
#include "Time.h"
#include "WindowDetails.h"
#include "OriginalRenderer.h"
#include "OriginalWindow.h"
#include "InputManager.h"
#include "ImGuiIncludes.h"
#include "ProjectLoader.h"
#include "World.h"
#include "UI.h"
#include "Editor.h"
#include "Camera.h"
#include "PhysicsWorld.h"

///Includes specifically for the file opening screen.
#include <ShlObj_core.h>
#include <SDL_syswm.h>

IWindow* Engine::m_Window = nullptr;
IRenderer* Engine::m_Renderer = nullptr;
GAME_STATE Engine::m_GameState = GAME_STATE::STOPPED;
std::string Engine::m_EngineEXELocation = "";

const std::string& Engine::GetEXELocation()
{
	return m_EngineEXELocation;
}

IRenderer& Engine::GetRenderer()
{
	if (!m_Renderer)
	{
		Console::LogMessage(LogLevel::LOG_WARNING, "Tried to get the renderer when it is not initalised yet.");
	}

	return *m_Renderer;
}

IWindow& Engine::GetWindow()
{
	if (!m_Window)
	{
		Console::LogMessage(LogLevel::LOG_WARNING, "Tried to get the window when it is not initalised yet.");
	}

	return *m_Window;
}

Engine::Engine()
{
	m_IsInEngineMode = false;
	m_EngineEXELocation = "";
	m_IsInitialised = false;
	m_IsRunning = false;
	m_Window = nullptr;
	m_Renderer = nullptr;
	m_AutosaveEnabled = true;
	m_AutosaveTimer = 0.0f;
	m_AutosaveCooldown = 15.0f;
}

Engine::~Engine()
{
	if (m_IsInitialised)
		Shutdown();
}

void Engine::Start(const ENGINE_MODE& mode)
{
	if(GetIsInitialised())
	{
		mode == ENGINE_MODE::EDITOR ? m_IsInEngineMode = true : m_IsInEngineMode = false;

		SetIsRunning(true);

		const int FPS = 60;
		const int frameDelay = 1000 / FPS;
		Uint32 currentTime = 0, deltaTimeMilliseconds = 0, oldTime = 0;
		Uint32 frameTime = 0;
		float DeltaTimeInSeconds = 0.0;
		Time::Get(DeltaTimeInSeconds);

		while(GetIsRunning())
		{
			oldTime = SDL_GetTicks();

			while (GetIsRunning())
			{
				currentTime = SDL_GetTicks();

				//frame delta in miliseconds
				deltaTimeMilliseconds = currentTime - oldTime;

				DeltaTimeInSeconds = deltaTimeMilliseconds / 1000.0f;

				if (deltaTimeMilliseconds != 0)
				{
					HandleEvents();
					Update(DeltaTimeInSeconds);
					Render();
				}

				frameTime = SDL_GetTicks() - currentTime;
				if (frameDelay > frameTime)
					SDL_Delay(frameDelay - frameTime);

				oldTime = currentTime;
			}
		}
	}
}

const GAME_STATE Engine::GetGameState()
{
	return Engine::m_GameState;
}


void Engine::Initialise(int argc, char* argv[], WindowDetails details)
{
	m_IsInitialised = (InitialiseSystems(details, argc, argv) && InitialiseApplicationControls());

	if(m_IsInitialised == false)
	{
		Shutdown();
	}
}

bool Engine::InitialiseGraphics()
{
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	if (!m_Window || m_Window == nullptr)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	if (m_Renderer)
	{
		m_Renderer->Shutdown();

		delete m_Renderer;
		m_Renderer = nullptr;
	}

	m_Renderer = new OriginalRenderer();
	m_Renderer->Startup(*m_Window);

	if (m_Renderer->IsInitialised() == false)
	{
		m_Renderer->Shutdown();
		m_Renderer = nullptr;
		return false;
	}

	return true;
}

bool Engine::InitialiseApplicationControls()
{
	InputManager::Bind(IM_KEY_CODE::IM_KEY_F1, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			m_Renderer->TakeScreenshot("");
		}); 

	InputManager::Bind(IM_KEY_CODE::IM_KEY_F2, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			if (Console::IsVisible())
			{
				Console::Hide();
			}
			else
			{
				Console::Show();
			}
		});

	return true;
}

bool Engine::InitialiseWindow(const WindowDetails& details)
{
	if (m_Window)
	{
		if (m_Window->IsInitialised())
		{
			m_Window->Shutdown();
			delete m_Window;
			m_Window = nullptr;
		}
	}

	m_Window = new OriginalWindow();
	m_Window->Startup(details);

	return m_Window->IsInitialised();
}

bool Engine::InitialiseSystems(const WindowDetails& details, int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		if (TTF_Init() < 0)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Console::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}

		Console::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");

		if (InitialiseWindow(details) == false)
			return false;

		if (InitialiseGraphics() == false)
			return false;
	
		if (argv != nullptr)
		{
			m_EngineEXELocation = argv[0];
		}

		return true;
	}
	
	return false;
}

void Engine::Shutdown()
{
	// Cleanup
	if(m_Renderer) m_Renderer->Shutdown();
	if(m_Window) m_Window->Shutdown();

	m_IsInitialised = false;
}

void Engine::HandleEvents()
{
	SDL_Event event;
	bool hadWheelEvent = false;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		if(!hadWheelEvent)
		{
			if (event.type == SDL_MOUSEWHEEL)
			{
				hadWheelEvent = true;

				if (event.wheel.y > 0)
				{
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_UP);
				}
				else if (event.wheel.y < 0)
				{
					InputManager::Get()->MouseScrollUpdate(IM_SCROLL_DIRECTION::IM_SCROLL_DOWN);
				}
			}
		}

		switch (event.type)
		{
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				InputManager::Get()->MousePressUpdate(event.button.button, true);
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				InputManager::Get()->MousePressUpdate(event.button.button, false);
			}
				break;

		case SDL_MOUSEMOTION:
			//Get mouse Position
			int x, y;
			SDL_GetMouseState(&x, &y);
			InputManager::Get()->MousePositionUpdate(x, y);
			break;
			
		case SDL_QUIT:
			m_IsRunning = false;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//Call to getwindowsize to update the console variables.
				Vector2 size;
				m_Window->GetWindowSize(size);
			}
			break;

		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				m_IsRunning = false;
			else
				InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, true);
			break;

		case SDL_KEYUP:
			InputManager::Get()->KeyPressUpdate(event.key.keysym.sym, false);
			break;

		default:
			break;
		}
	}
}

void Engine::Update(float DeltaTime)
{
	if (ProjectLoader::HasProjectLoaded() && m_GameState != GAME_STATE::RUNNING)
	{
		if (m_AutosaveEnabled)
		{
			m_AutosaveTimer += DeltaTime;

			if (m_AutosaveTimer >= m_AutosaveCooldown)
			{
				ProjectLoader::SaveProject();
				m_AutosaveTimer = 0.0f;
			}
		}
	}

	InputManager::Update();
	
	if(m_GameState == GAME_STATE::RUNNING)
		Physics::Update(DeltaTime);

	World::Update(DeltaTime);
	UI::Update(DeltaTime);

	if(Console::Query("DrawLog") != 0)
		Console::Update(DeltaTime);

	Editor::Update(DeltaTime);
}

void Engine::Render()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

	m_Renderer->StartFrame();

	World::Render(*m_Renderer);
	UI::Render(*m_Renderer);

	if (Console::Query("DrawLog") != 0)
		Console::Render(*m_Renderer);

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
					m_IsRunning = false;
				}

				ImGui::Dummy(Vector2(-FLT_MAX, 100.0f));

				if (ImGui::MenuItem("Exit without Saving"))
				{
					m_IsRunning = false;
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
			if(ImGui::IsPopupOpen("New Project") == false)
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

	Editor::Render(*m_Renderer);

	ImGui::Begin("Render Window", 0, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	static bool showDeletionConfirmation = false;
	ImGui::BeginMenuBar();
	{
		switch (m_GameState)
		{
		case GAME_STATE::RUNNING:
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			if (ImGui::Button("Stop"))
			{
				m_GameState = GAME_STATE::STOPPED;
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
				m_GameState = GAME_STATE::RUNNING;
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
	OriginalRenderer* renderer = (OriginalRenderer*)m_Renderer;

	SDL_SetRenderTarget(renderer->GetAPIRenderer(), NULL);
	Vector2 size;
	size.X = ImGui::GetWindowWidth();
	size.Y = ImGui::GetWindowHeight();
	Vector2 pos;
	pos.X = ImGui::GetWindowPos().x;
	pos.Y = ImGui::GetWindowPos().y;
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();/*

	Console::Run("WindowDimensionsW " + std::to_string(size.X));
	Console::Run("WindowDimensionsH " + std::to_string(size.Y));
	Console::Run("WindowCentreX " + std::to_string(size.X / 2));
	Console::Run("WindowCentreY " + std::to_string(size.Y / 2));*/

	SDL_Rect renderDstQuad = { (int)(pos.X + vMin.x), (int)(pos.Y + vMin.y), (int)(vMax.x - vMin.x), (int)(vMax.y - vMin.y) };

	SDL_Rect renderSrcQuad = renderDstQuad;
	renderSrcQuad.w /= (int)Camera::ZoomLevel;
	renderSrcQuad.h /= (int)Camera::ZoomLevel;

	SDL_RenderSetClipRect(renderer->GetAPIRenderer(), &renderDstQuad);
	//Render to screen
	ImGui::End();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	
	if (showDeletionConfirmation == false && showNewProjectModal == false && showOpenProjectModal == false)
	{
		SDL_RenderCopyEx(renderer->GetAPIRenderer(), renderer->GetRenderTexture(), &renderSrcQuad, &renderDstQuad, 0.0F, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
	}

	m_Renderer->EndFrame();
}

// callback function
INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
	return 0;
}

bool Engine::OpenProject(std::string& path)
{
	//todo : sortout
	//This is not abstracted :(
	SDL_SysWMinfo wmInfo{};
	SDL_VERSION(&wmInfo.version);
	OriginalWindow* window = dynamic_cast<OriginalWindow*>(m_Window);
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
#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "World.h"
#include "Log.h"
#include "Time.h"
#include "PhysicsWorld.h"
#include <iostream>
#include "UI.h"
#include "Editor.h"
#include "Console.h"
#include "ProjectLoader.h"
#include <filesystem>
#include <ShlObj_core.h>
#include <SDL_syswm.h>
#include <SDL.h>
#include "Camera.h"

SDL_Renderer* Game::Renderer = nullptr;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}

int main(int argc, char* argv[])
{
	WindowDetails details;
	details.Dimensions = Vector2(1280.0f, 720.0f);
	details.Title = "hTech";
	details.Position = Vector2(200.0f, 200.0f);

	Game* game = new Game();
	game->Initialise(argc, argv, details);

	if (game->GetIsInitialised())
	{
		game->Start();
	}

	return 0;
}

Game::Game()
{
	m_IsInitialised = false;
	m_IsRunning = false;
	m_Window = nullptr;
	m_RenderToTextureTarget = nullptr;
}

Game::~Game()
{
	if (m_IsInitialised)
		Shutdown();
}

void Game::Start()
{
	if(GetIsInitialised())
	{
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

/// <summary>
/// Sets the window state and resizes to the enum state passed in.
/// </summary>
/// <param name="state">SCREEN_STATE enum defining either fullscreen, borderless fullscreen or windowed mode.</param>
void Game::SetFullscreen(SCREEN_STATE state)
{
	switch (state)
	{
	case SCREEN_STATE::WINDOW_FULLSCREEN:
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto Width = DM.w;
		auto Height = DM.h;
		SDL_SetWindowSize(m_Window, Width, Height);
		SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN);
	}
		break;
	case SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN:
		SDL_SetWindowFullscreen(m_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case SCREEN_STATE::WINDOW_WINDOWED:
		SDL_SetWindowFullscreen(m_Window, 0);
		break;
	}

	int w, h;
	SDL_GetWindowSize(m_Window, &w, &h);
	Console::Run("WindowDimensionsW " + std::to_string(w));
	Console::Run("WindowDimensionsH " + std::to_string(h));

	Console::Run("WindowCentreX " + std::to_string(w / 2));
	Console::Run("WindowCentreY " + std::to_string(h / 2));
}

/// <summary>
/// Creates a screenshot of the current frame.
/// </summary>
/// <param name="name">Name of the output file.</param>
void Game::TakeScreenshot(std::string name)
{
	int w, h;
	w = Console::Query("WindowDimensionsW");
	h = Console::Query("WindowDimensionsH");

	SDL_Surface* sshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(Game::Renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

	struct tm _time;
	std::string str = name;
	time_t now = time(nullptr);
	localtime_s(&_time, &now);
	char* buffer = new char[256];
	strftime(buffer, 256, "%d-%m-%Y %H-%M-%S", &_time);

	if (str == "")
	{
		str = buffer;
		str += ".bmp";
	}

	SDL_SaveBMP(sshot, str.c_str());
	SDL_FreeSurface(sshot);

	str = "Screenshot taken: " + str;

	Log::LogMessage(LogLevel::LOG_MESSAGE, str.c_str());
}

void Game::Initialise(int argc, char* argv[], WindowDetails details)
{
	m_IsInitialised = (InitialiseSystems(details) && InitialiseApplicationControls());

	if(m_IsInitialised == false)
	{
		Shutdown();
	}

	Console::Run("DrawHashMap 1");
}

bool Game::InitialiseWindow(const char* Title, int xpos, int ypos, int width, int height, unsigned int flags, bool isFullscreen)
{
	if (m_Window)
		SDL_DestroyWindow(m_Window);
		
	if (Title == "")
		Title = "No Window Title Given";

	if (xpos == 0)
		xpos = SDL_WINDOWPOS_CENTERED;

	if (ypos == 0)
		ypos = SDL_WINDOWPOS_CENTERED;

	if (width == 0)
		width = 800;

	if (height == 0)
		height = 600;

	if (isFullscreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	//For IMGUI
	flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

	m_Window = SDL_CreateWindow(Title, xpos, ypos, width, height, flags);

	if (m_Window)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Window created.");
		return true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Window failed to create.");
		return false;
	}
}

bool Game::InitialiseDearIMGUI()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigDockingWithShift = true;
	io.ConfigFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	SettingDearIMGUIColourScheme();

	// Setup Platform/Renderer backends
	return (ImGui_ImplSDL2_InitForSDLRenderer(m_Window, Renderer) && ImGui_ImplSDLRenderer_Init(Renderer));
}

void Game::SettingDearIMGUIColourScheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 5.3f;
	style.FrameRounding = 2.3f;
	style.ScrollbarRounding = 0;

	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 0.90f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

bool Game::InitialiseGraphics()
{
	if (!m_Window || m_Window == nullptr)
		return false;

	SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");

	if (Game::Renderer)
		SDL_DestroyRenderer(Game::Renderer);

	Game::Renderer = SDL_CreateRenderer(m_Window, -1, 0);

	if (Game::Renderer)
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Renderer created.");

#ifdef _DEBUG
		if (!CreateRenderTargetTexture())
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Failed to render target texture.");
			Log::LogMessage(LogLevel::LOG_ERROR, SDL_GetError());
			return false;
		}

		Log::LogMessage(LogLevel::LOG_MESSAGE, "Created render target texture.");
#endif

		if (InitialiseDearIMGUI())
		{
			Log::LogMessage(LogLevel::LOG_MESSAGE, "DearIMGUI initialised.");

			return true;
		}
		
		return false;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Renderer failed to create.");
		return false;
	}
}

bool Game::CreateRenderTargetTexture()
{
	bool success = true;

	if (m_RenderToTextureTarget != nullptr)
	{
		SDL_DestroyTexture(m_RenderToTextureTarget);
	}

	int w, h;
	SDL_GetWindowSize(m_Window, &w, &h);

	m_RenderToTextureTarget = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

	if (!m_RenderToTextureTarget)
	{
		success = false;
	}
	
	return success;
}

bool Game::InitialiseApplicationControls()
{
	InputManager::Bind(IM_KEY_CODE::IM_KEY_F1, IM_KEY_STATE::IM_KEY_PRESSED,
		[this]
		{
			TakeScreenshot("");
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

bool Game::InitialiseSystems(WindowDetails details)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		if (TTF_Init() < 0)
		{
			Log::LogMessage(LogLevel::LOG_ERROR, "Error initializing SDL_ttf");
			Log::LogMessage(LogLevel::LOG_ERROR, TTF_GetError());
		}

		Log::LogMessage(LogLevel::LOG_MESSAGE, "Subsystem created.");

		if (InitialiseWindow(details.Title.c_str(), (int)details.Position.X, (int)details.Position.Y, (int)details.Dimensions.X, (int)details.Dimensions.Y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_OPENGL, false) == false)
			return false;

		int w, h;
		SDL_GetWindowSize(m_Window, &w, &h);
		Console::Run("WindowDimensionsW " + std::to_string(w));
		Console::Run("WindowDimensionsH " + std::to_string(h));
		Console::Run("WindowCentreX " + std::to_string(w / 2));
		Console::Run("WindowCentreY " + std::to_string(h / 2));

		if (InitialiseGraphics() == false)
			return false;

		SDL_SetRenderDrawBlendMode(Game::Renderer, SDL_BLENDMODE_BLEND);
			
		return true;
	}
	else
		return false;
}

void Game::Shutdown()
{
	// Cleanup
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(Game::Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();

	m_IsInitialised = false;
}

void Game::SetPlayState(GAME_STATE state)
{
	m_GameState = state;
}

GAME_STATE Game::GetPlayState()
{
	return m_GameState;
}

void Game::HandleEvents()
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
				int w, h;
				SDL_GetWindowSize(m_Window, &w, &h);
				Console::Run("WindowDimensionsW " + std::to_string(w));
				Console::Run("WindowDimensionsH " + std::to_string(h));
				Console::Run("WindowCentreX " + std::to_string(w / 2));
				Console::Run("WindowCentreY " + std::to_string(h / 2));
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

void Game::Update(float DeltaTime)
{
	InputManager::Update();

	if (m_GameState == GAME_STATE::Playing)
	{
		Physics::Update(DeltaTime);
		World::Update(DeltaTime);
		UI::Update(DeltaTime);
	}

	if(Console::Query("DrawLog") != 0)
		Log::Update(DeltaTime);

	Editor::Update(DeltaTime);
}

void Game::Render()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);

	SDL_RenderClear(Renderer);
	SDL_SetRenderTarget(Renderer, m_RenderToTextureTarget);
	SDL_SetRenderDrawColor(Renderer, 29, 49, 68, 255);
	SDL_RenderClear(Renderer);

	World::Render(*Renderer);
	UI::Render(*Renderer);

	if (Console::Query("DrawLog") != 0)
		Log::Render(*Renderer);

#ifdef _DEBUG
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
					SetPlayState(GAME_STATE::Paused);
					ProjectLoader::LoadProject(projectFilePath);
				}
			}

			if (ImGui::MenuItem("Save Project"))
			{
				SetPlayState(GAME_STATE::Paused);
				Log::LogMessage(LogLevel::LOG_MESSAGE, "Paused to save project...");
				ProjectLoader::SaveProject();
			}
			
			if (ImGui::MenuItem("Close Project"))
			{
				ProjectLoader::UnloadProject();
				SetPlayState(GAME_STATE::Paused);
			}

			if (ImGui::BeginMenu("Exit##Menu"))
			{
				if (ImGui::MenuItem("Exit with Saving"))
				{
					SetPlayState(GAME_STATE::Paused);
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

		switch (m_GameState)
		{
		case GAME_STATE::Paused:
			if (ImGui::Button("Start Playing"))
			{
				m_GameState = GAME_STATE::Playing;
			}
			break;
		case GAME_STATE::Playing:
			if (ImGui::Button("Stop Playing"))
			{
				m_GameState = GAME_STATE::Paused;
			}
			break;
		default:
			break;
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
					SetPlayState(GAME_STATE::Paused);
					ProjectLoader::CreateProject(projectName);
					projectName = "";
					showNewProjectModal = false;
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

	Editor::Render(*Renderer);

	SDL_SetRenderTarget(Renderer, NULL);

	ImGui::Begin("Render Window", 0, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

	ImGui::BeginMenuBar();
	{
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

	ImGui::EndMenuBar();

	Vector2 size;
	size.X = ImGui::GetWindowWidth();
	size.Y = ImGui::GetWindowHeight();
	Vector2 pos;
	pos.X = ImGui::GetWindowPos().x;
	pos.Y = ImGui::GetWindowPos().y;
	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	ImVec2 vMax = ImGui::GetWindowContentRegionMax();

	Console::Run("WindowDimensionsW " + std::to_string(size.X));
	Console::Run("WindowDimensionsH " + std::to_string(size.Y));
	Console::Run("WindowCentreX " + std::to_string(size.X / 2));
	Console::Run("WindowCentreY " + std::to_string(size.Y / 2));

	SDL_Rect renderQuad = { (int)(pos.X + vMin.x), (int)(pos.Y + vMin.y), (int)(vMax.x - vMin.x), (int)(vMax.y - vMin.y) };
	SDL_RenderSetClipRect(Renderer, &renderQuad);
	//Render to screen
	SDL_Point center = { (int)(renderQuad.x + (renderQuad.w / 2)),  (int)(renderQuad.y + (renderQuad.h / 2)) };
	ImGui::End();

	ImGui::Render();
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	if (showNewProjectModal == false)
	{
		SDL_RenderCopyEx(Renderer, m_RenderToTextureTarget, nullptr, nullptr, 0.0F, &center, SDL_RendererFlip::SDL_FLIP_NONE);
	}

#endif

	SDL_RenderPresent(Game::Renderer);
}

// callback function
INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
	if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
	return 0;
}

bool Game::OpenProject(std::string& path)
{
	SDL_SysWMinfo wmInfo{};
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(m_Window, &wmInfo);

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
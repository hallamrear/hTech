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
#include "ScriptLoader.h"
#include "World.h"
#include "UI.h"
#include "Editor.h"
#include "Camera.h"
#include "PhysicsWorld.h"

IWindow* Engine::m_Window = nullptr;
IRenderer* Engine::m_Renderer = nullptr;
GAME_STATE Engine::m_GameState = GAME_STATE::STOPPED;
std::string Engine::m_EngineEXELocation = "";
ENGINE_MODE Engine::m_EngineMode = ENGINE_MODE::PLAYER;
bool Engine::m_IsRunning = false;

ENGINE_MODE Engine::GetEngineMode()
{
	return m_EngineMode;
}

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
	m_EngineEXELocation = "";
	m_IsInitialised = false;
	m_Window = nullptr;
	m_Renderer = nullptr;
}

Engine::~Engine()
{
	if (m_IsInitialised)
		Shutdown();
}

void Engine::Start(const char* ProjectToOpenPath)
{
	if(GetIsInitialised())
	{
		if (m_EngineMode == ENGINE_MODE::PLAYER)
		{
			m_GameState = GAME_STATE::RUNNING;

			if (ProjectToOpenPath != "")
			{
				ProjectLoader::LoadProject(ProjectToOpenPath);
				World::ResetWorldEntities();
			}
			else
			{
				Console::LogMessage(LogLevel::LOG_ERROR, "Engine loaded in Player mode but the path for the project cannot be found.");
				Console::LogMessage(LogLevel::LOG_ERROR, std::string("Path: ") + ProjectToOpenPath);
				return;
			}
		}

		


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

void Engine::Initialise(int argc, char* argv[], WindowDetails details, const ENGINE_MODE& mode)
{
	m_EngineMode = mode;

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

	if (GetEngineMode() == ENGINE_MODE::EDITOR)
	{
		InputManager::Bind(IM_KEY_CODE::IM_KEY_UP_ARROW, IM_KEY_STATE::IM_KEY_HELD,
			[this]
			{
				Vector2 pos = Camera::GetCameraPosition();
				pos.Y += 2.5f;
				Camera::SetCameraPosition(pos);
			});

		InputManager::Bind(IM_KEY_CODE::IM_KEY_DOWN_ARROW, IM_KEY_STATE::IM_KEY_HELD,
			[this]
			{
				Vector2 pos = Camera::GetCameraPosition();
				pos.Y -= 2.5f;
				Camera::SetCameraPosition(pos);
			});

		InputManager::Bind(IM_KEY_CODE::IM_KEY_LEFT_ARROW, IM_KEY_STATE::IM_KEY_HELD,
			[this]
			{
				Vector2 pos = Camera::GetCameraPosition();
				pos.X -= 2.5f;
				Camera::SetCameraPosition(pos);
			});

		InputManager::Bind(IM_KEY_CODE::IM_KEY_RIGHT_ARROW, IM_KEY_STATE::IM_KEY_HELD,
			[this]
			{
				Vector2 pos = Camera::GetCameraPosition();
				pos.X += 2.5f;
				Camera::SetCameraPosition(pos);
			});
	}

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

	switch (m_EngineMode)
	{
	case ENGINE_MODE::EDITOR:
		Editor::Render(*m_Renderer);
		break;
	case ENGINE_MODE::PLAYER:
	{
		OriginalRenderer* castedRenderer = (OriginalRenderer*)m_Renderer;
		SDL_SetRenderTarget(castedRenderer->GetAPIRenderer(), NULL);
		SDL_RenderCopyEx(castedRenderer->GetAPIRenderer(), castedRenderer->GetRenderTexture(), NULL, NULL, 0.0F, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
		ImGui::Render();
		ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	}
		break;
	default:
		break;
	}

	m_Renderer->EndFrame();
}

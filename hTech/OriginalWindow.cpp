#include "pch.h"
#include "OriginalWindow.h"
#include "Log.h"
#include "Console.h"


OriginalWindow::OriginalWindow() : IWindow()
{
	m_SDLWindow = nullptr;
	m_IsInitialised = false;
}

OriginalWindow::~OriginalWindow()
{
	if (m_IsInitialised)
	{
		Shutdown();
	}
}

void OriginalWindow::Startup(const WindowDetails& details)
{
	if (m_IsInitialised)
	{
		Shutdown();
	}

	WindowDetails windowData = WindowDetails(details);

	if (m_SDLWindow)
		SDL_DestroyWindow(m_SDLWindow);

	if (windowData.Title == "")
		windowData.Title = "No Window Title Given";

	if (windowData.Position.X == 0)
		windowData.Position.X = SDL_WINDOWPOS_CENTERED;

	if (windowData.Position.Y == 0)
		windowData.Position.Y = SDL_WINDOWPOS_CENTERED;

	if (windowData.Dimensions.X == 0)
		windowData.Dimensions.X = 800;

	if (windowData.Dimensions.Y == 0)
		windowData.Dimensions.Y = 600;

	int flags = 0;

	switch (windowData.StartupState)
	{
	case SCREEN_STATE::WINDOW_FULLSCREEN:
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	break;

	case SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN:
	{
		flags |= SDL_WINDOW_BORDERLESS;
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	break;

	case SCREEN_STATE::WINDOW_WINDOWED:
	{

	}
	break;

	default:
		break;
	}

	//For IMGUI
	flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	flags |= SDL_WINDOW_OPENGL;
	flags |= SDL_WINDOW_SHOWN;
	flags |= SDL_WINDOW_RESIZABLE;
	flags |= SDL_WINDOW_MAXIMIZED;

	m_SDLWindow = SDL_CreateWindow(windowData.Title.c_str(), (int)windowData.Position.X, (int)windowData.Position.Y, (int)windowData.Dimensions.Y, (int)windowData.Dimensions.Y, flags);

	if (m_SDLWindow)
	{
		Vector2 size;
		GetWindowSize(size);
	
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Window created.");
		m_IsInitialised = true;
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_ERROR, "Window failed to create.");
		SDL_DestroyWindow(m_SDLWindow);
	}
}

void OriginalWindow::Shutdown()
{
	if (m_IsInitialised)
	{
		SDL_DestroyWindow(m_SDLWindow);
		SDL_Quit();
	}
}

void OriginalWindow::SetWindowSize(const Vector2& size)
{
	if (!m_IsInitialised)
		return;

	int w = (int)size.X;
	int h = (int)size.Y;
	SDL_SetWindowSize(m_SDLWindow, w, h);
	Console::Run("WindowDimensionsW " + std::to_string(w));
	Console::Run("WindowDimensionsH " + std::to_string(h));
	Console::Run("WindowCentreX " + std::to_string(w / 2));
	Console::Run("WindowCentreY " + std::to_string(h / 2));
}

void OriginalWindow::GetWindowSize(Vector2& size)
{
	if (!m_IsInitialised)
		return;

	int w, h;
	SDL_GetWindowSize(m_SDLWindow, &w, &h);
	size.X = w;
	size.Y = h;
	Console::Run("WindowDimensionsW " + std::to_string(w));
	Console::Run("WindowDimensionsH " + std::to_string(h));
	Console::Run("WindowCentreX " + std::to_string(w / 2));
	Console::Run("WindowCentreY " + std::to_string(h / 2));
}


/// <summary>
/// Sets the window state and resizes to the enum state passed in.
/// </summary>
/// <param name="state">SCREEN_STATE enum defining either fullscreen, borderless fullscreen or windowed mode.</param>
void OriginalWindow::SetFullscreenState(SCREEN_STATE state)
{
	if (!m_IsInitialised)
		return;

	switch (state)
	{
	case SCREEN_STATE::WINDOW_FULLSCREEN:
	{
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		auto Width = DM.w;
		auto Height = DM.h;
		SDL_SetWindowSize(m_SDLWindow, Width, Height);
		SDL_SetWindowFullscreen(m_SDLWindow, SDL_WINDOW_FULLSCREEN);
	}
	break;
	case SCREEN_STATE::WINDOW_BORDERLESS_FULLSCREEN:
		SDL_SetWindowFullscreen(m_SDLWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		break;
	case SCREEN_STATE::WINDOW_WINDOWED:
		SDL_SetWindowFullscreen(m_SDLWindow, 0);
		break;
	}

	//Call get window size to update the console variables.
	Vector2 size;
	GetWindowSize(size);
}

void OriginalWindow::SetWindowTitle(const std::string& title)
{
	SDL_SetWindowTitle(m_SDLWindow, title.c_str());
}

void OriginalWindow::SetWindowTitle(const char* title)
{
	SDL_SetWindowTitle(m_SDLWindow, title);
}

void OriginalWindow::GetWindowTitle(std::string& title)
{
	title = SDL_GetWindowTitle(m_SDLWindow);
}

void OriginalWindow::SetWindowPosition(const Vector2& position)
{
	//todo : add console values here
	//they apparently dont exist in the console.
	SDL_SetWindowPosition(m_SDLWindow, (int)position.X, (int)position.Y);
}

void OriginalWindow::GetWindowPosition(Vector2& position)
{
	//todo : add console values here
	//they apparently dont exist in the console.
	SDL_GetWindowPosition(m_SDLWindow, (int*)&position.X, (int*)&position.Y);
}

SDL_Window* OriginalWindow::GetAPIWindow()
{
	return m_SDLWindow;
}

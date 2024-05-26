#pragma once
#include "Rendering/IWindow.h"

struct SDL_Window;

class OriginalWindow : public IWindow
{
private:
	SDL_Window* m_SDLWindow;

public:
	OriginalWindow();
	~OriginalWindow();

	void Startup(const WindowDetails& details);
	void Shutdown();

	void SetWindowSize(const Vector2& size);
	void GetWindowSize(Vector2& size) const;
	void SetFullscreenState(SCREEN_STATE state);
	void SetWindowTitle(const std::string& title);
	void SetWindowTitle(const char* title);
	void GetWindowTitle(std::string& title) const;
	void SetWindowPosition(const Vector2& position);
	void GetWindowPosition(Vector2& position) const;

	SDL_Window* GetAPIWindow();
};
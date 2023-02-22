#pragma once
#include <string>

enum class HTECH_FUNCTION_EXPORT SCREEN_STATE
{
	WINDOW_FULLSCREEN = 0,
	WINDOW_BORDERLESS_FULLSCREEN,
	WINDOW_WINDOWED
};

class HTECH_FUNCTION_EXPORT WindowDetails
{
public:
	std::string Title = "";
	Vector2 Position = Vector2();
	Vector2 Dimensions = Vector2(600.0f, 300.0f);
	SCREEN_STATE StartupState = SCREEN_STATE::WINDOW_WINDOWED;
};

class IWindow
{
private:

protected:
	bool m_IsInitialised;

public:
	IWindow();
	virtual ~IWindow() = 0;

	const bool IsInitialised() const;

	virtual void Startup(const WindowDetails& details) = 0;
	virtual void Shutdown() = 0;

	virtual void SetWindowTitle(const std::string& title) = 0;
	virtual void SetWindowTitle(const char* title) = 0;
	virtual void GetWindowTitle(std::string& title) = 0;
	virtual void SetWindowPosition(const Vector2& position) = 0;
	virtual void GetWindowPosition(Vector2& position) = 0;
	virtual void SetWindowSize(const Vector2& size) = 0;
	virtual void GetWindowSize(Vector2& size) = 0;
	virtual void SetFullscreenState(SCREEN_STATE state) = 0;
};


#pragma once
#include <string>
#include "WindowDetails.h"

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


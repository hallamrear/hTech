#pragma once

enum class HTECH_FUNCTION_EXPORT SCREEN_STATE
{
	WINDOW_FULLSCREEN = 0,
	WINDOW_BORDERLESS_FULLSCREEN,
	WINDOW_WINDOWED
};

struct HTECH_FUNCTION_EXPORT WindowDetails
{
public:
	std::string Title = "";
	Vector2 Position = Vector2();
	Vector2 Dimensions = Vector2(1280.0f, 720.0f);
	SCREEN_STATE StartupState = SCREEN_STATE::WINDOW_WINDOWED;
};
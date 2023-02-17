#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>
#include "Vector2.h"

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
};

enum GAME_STATE : int
{
	STOPPED = 0,
	PAUSED = 1,
	RUNNING = 2
};

class HTECH_FUNCTION_EXPORT Game
{
private:
	bool  m_AutosaveEnabled;
	float m_AutosaveTimer;
	float m_AutosaveCooldown;

	static GAME_STATE			m_GameState;
	bool						m_IsInitialised;
	bool						m_IsRunning;
	struct SDL_Window*			m_Window;
	struct SDL_Texture*			m_RenderToTextureTarget;
	 
	bool		InitialiseGraphics();
	bool		CreateRenderTargetTexture();
															
	//4 == SDL_WINDOW_SHOWN 
	bool		InitialiseWindow(const char* Title = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, unsigned int flags = 4, bool isFullscreen = false);
	bool		InitialiseDearIMGUI();
	void		SettingDearIMGUIColourScheme();
	bool		InitialiseApplicationControls();
	bool		InitialiseSystems(WindowDetails details);

	void		HandleEvents();
	void		Update(float DeltaTime);
	void		Render();
	void		Shutdown();


public:
	static struct SDL_Renderer* Renderer;

	Game();
	~Game();

	void		Initialise(int argc, char* argv[], WindowDetails details);
	void		Start();

	static const GAME_STATE  GetGameState();
	void		SetFullscreen(SCREEN_STATE state);
	void	    SetIsRunning(bool state) { m_IsRunning = state; };
	const bool  GetIsRunning() const { return m_IsRunning; };
	const bool  GetIsInitialised() const { return m_IsInitialised; };

	void		TakeScreenshot(std::string name);
	bool		OpenProject(std::string& path);
};


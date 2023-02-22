#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>
#include "Vector2.h"

struct WindowDetails;
class IRenderer;
class IWindow;

enum GAME_STATE : int
{
	STOPPED = 0,
	PAUSED = 1,
	RUNNING = 2
};

class HTECH_FUNCTION_EXPORT Game
{
private:
	static IRenderer* m_Renderer;
	static IWindow*   m_Window;

	bool  m_AutosaveEnabled;
	float m_AutosaveTimer;
	float m_AutosaveCooldown;

	static GAME_STATE			m_GameState;
	bool						m_IsInitialised;
	bool						m_IsRunning;
	 
	bool		InitialiseWindow(const WindowDetails& details);
	bool		InitialiseSystems(const WindowDetails& details);
	bool		InitialiseApplicationControls();
	bool		InitialiseGraphics();

	void		HandleEvents();
	void		Update(float DeltaTime);
	void		Render();
	void		Shutdown();


public:

	static IRenderer& GetRenderer();
	static IWindow&   GetWindow();

	Game();
	~Game();

	void		Initialise(int argc, char* argv[], WindowDetails details);
	void		Start();

	static const GAME_STATE  GetGameState();
	void	    SetIsRunning(bool state) { m_IsRunning = state; };
	const bool  GetIsRunning() const { return m_IsRunning; };
	const bool  GetIsInitialised() const { return m_IsInitialised; };
	bool		OpenProject(std::string& path);
};


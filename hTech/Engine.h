#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#include <string>
#include "Vector2.h"

struct WindowDetails;
class IRenderer;
class IWindow;

enum class GAME_STATE : int
{
	STOPPED = 0,
	PAUSED = 1,
	RUNNING = 2
};

enum class ENGINE_MODE : int
{
	EDITOR = 0,
	PLAYER = 1
};

class HTECH_FUNCTION_EXPORT Engine
{
private:
	static std::string			m_EngineEXELocation;
	static ENGINE_MODE			m_EngineMode;
	static IRenderer*			m_Renderer;
	static IWindow*				m_Window;

	static GAME_STATE			m_GameState;
	bool						m_IsInitialised;
	static bool					m_IsRunning;
	 
	bool		InitialiseWindow(const WindowDetails& details);
	bool		InitialiseSystems(const WindowDetails& details, int argc, char* argv[]);
	bool		InitialiseApplicationControls();
	bool		InitialiseGraphics();

	void		HandleEvents();
	void		Update(float DeltaTime);
	void		Render();
	void		Shutdown();


public:
	static ENGINE_MODE GetEngineMode();
	static const std::string& GetEXELocation();
	static IRenderer& GetRenderer();
	static IWindow&   GetWindow();

	Engine();
	~Engine();

	void		Initialise(int argc, char* argv[], WindowDetails details, const ENGINE_MODE& mode);
	void		Start(const char* ProjectToOpenPath);

	static const GAME_STATE  GetGameState();
	static const void		 SetGameState(const GAME_STATE& state) { m_GameState = state; };
	static void				 SetIsRunning(bool state) { m_IsRunning = state; };
	static const bool		 GetIsRunning() { return m_IsRunning; };
	const bool				 GetIsInitialised() const { return m_IsInitialised; };
};


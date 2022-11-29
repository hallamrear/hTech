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
	std::string title = "";
	Vector2 position = Vector2();
	Vector2 dimensions = Vector2(600.0f, 300.0f);
};

class HTECH_FUNCTION_EXPORT Game
{
private:
	bool						mIsInitialised;
	bool						mIsRunning;

	struct SDL_Window*			mWindow;

	bool		InitialiseGraphics();
															
	//4 == SDL_WINDOW_SHOWN 
	bool		InitialiseWindow(const char* title = "", int xpos = 0, int ypos = 0, int width = 0, int height = 0, unsigned int flags = 4, bool isFullscreen = false);
	bool		InitialiseDearIMGUI();
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

	void		SetFullscreen(SCREEN_STATE state);
	void	    SetIsRunning(bool state) { mIsRunning = state; };
	const bool  GetIsRunning() const { return mIsRunning; };
	const bool  GetIsInitialised() const { return mIsInitialised; };

	void		TakeScreenshot(std::string name);
	void		CreateProjectFolder(std::string name);
	void		OpenProject();
	void		SaveProject();
};


#include "pch.h"
#include "Engine.h"
#include "BUILD_NUMBER.h"
#include "WindowDetails.h"
#include <string>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc = __argc;
	char* exeLocation = new char[512];
	char* projectLocation = new char[512];

	//Have to use LPWSTR for the commandline as there is no CommandLineToArgvA.
	LPWSTR* CMDLine = CommandLineToArgvW(GetCommandLineW(), &argc);
	wchar_t* argv = (wchar_t*)CMDLine[0];
	size_t convertedSize = 0;
	wcstombs_s(&convertedSize, exeLocation, 512, argv, 512);

	if (argc > 1)
	{
		argv = (wchar_t*)CMDLine[1];
		size_t convertedSize = 0;
		wcstombs_s(&convertedSize, projectLocation, 512, argv, 512);
	}

	WindowDetails details;
	details.Dimensions = Vector2(1280.0f, 720.0f);
	details.Title = "hTech | Build Number " + std::to_string(BUILD_NUMBER);
	details.Position = Vector2(200.0f, 200.0f);

	Engine* game = new Engine();
	ENGINE_MODE mode;
	argc > 1 ? mode = ENGINE_MODE::PLAYER : mode = ENGINE_MODE::EDITOR;

	game->Initialise(argc, __argv, details, mode);

	if (game->GetIsInitialised())
	{
		game->Start(projectLocation);
	}

	return 0;
}

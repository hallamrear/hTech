#include "pch.h"
#include "Engine.h"
#include "BUILD_NUMBER.h"
#include "WindowDetails.h"
#include <string>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc = __argc;

	//TODO : Check that this conversion from pointer to an array has not broken any of the copys.
	char exeLocation[512] = { '\0' };
	char projectLocation[512] = { '\0' };

	//Have to use LPWSTR for the commandline as there is no CommandLineToArgvA.
	LPWSTR* CMDLine = CommandLineToArgvW(GetCommandLineW(), &argc);
	wchar_t* argv = (wchar_t*)CMDLine[0];
	size_t convertedSize = 0;
	wcstombs_s(&convertedSize, &exeLocation[0], 512, argv, 512);

	if (argc > 1)
	{
		argv = (wchar_t*)CMDLine[1];
		size_t convertedSize = 0;
		wcstombs_s(&convertedSize, &projectLocation[0], 512, argv, 512);
	}

	WindowDetails details;
	details.Dimensions = Vector2(1280.0f, 720.0f);
	details.Title = "hTech | Build Number : TEMPORARILY DISABLED"; // +std::to_string(BUILD_NUMBER);
	details.Position = Vector2(200.0f, 200.0f);

	Engine* game = new Engine();

	//If the game has more than 1 argument,
	//it assumes that it is a project folder location and goes into editor mode.
	ENGINE_MODE mode;
	argc > 1 ? mode = ENGINE_MODE::PLAYER : mode = ENGINE_MODE::EDITOR;

	game->Initialise(argc, __argv, details, mode);

	if (game->GetIsInitialised())
	{
		game->Start(projectLocation);
	}

	return 0;
}

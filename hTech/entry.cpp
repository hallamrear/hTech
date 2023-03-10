#include "pch.h"
#include "Engine.h"
#include "BUILD_NUMBER.h"
#include "WindowDetails.h"

#include <string>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc = 0;
	char* argv = new char[512];

	//Have to use LPWSTR for the commandline as there is no CommandLineToArgvA.
	LPWSTR* CMDLine = CommandLineToArgvW(GetCommandLineW(), &argc);
	wchar_t* exeLocation = (wchar_t*)CMDLine[0];
	size_t convertedSize = 0;
	wcstombs_s(&convertedSize, argv, 512, exeLocation, 512);

	WindowDetails details;
	details.Dimensions = Vector2(1280.0f, 720.0f);
	details.Title = "hTech | Build Number " + std::to_string(BUILD_NUMBER);
	details.Position = Vector2(200.0f, 200.0f);

	Engine* game = new Engine();

	game->Initialise(argc, &argv, details);

	if (game->GetIsInitialised())
	{
		ENGINE_MODE mode;
		argc == 0 ? mode = ENGINE_MODE::PLAYER : mode = ENGINE_MODE::EDITOR;
		game->Start(mode);
	}

	return 0;
}

#include "pch.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"
#include <unordered_map>

static HINSTANCE mLoadedLibrary;
HINSTANCE ScriptLoader::mLoadedLibrary = nullptr;
std::unordered_map<std::string, ScriptObject*> ScriptLoader::mLoadedScriptMap = std::unordered_map<std::string, ScriptObject*>();

typedef ScriptObject* (__stdcall* scriptPtr)();

ScriptLoader::~ScriptLoader()
{
	UnloadLibrary();
}

void ScriptLoader::UnloadLibrary()
{
	if (mLoadedLibrary)
	{
		FreeLibrary(mLoadedLibrary);
	}
}

std::string aaaaaaaaaaaaaaaa()
{
	//Get the error message ID, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}

	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	std::cout << message << std::endl;

	return message;
};

void ScriptLoader::LoadCustomScriptDLL(std::string libraryLocation)
{
	UnloadLibrary();

	//get raw script location
	//HARDCODED : Needs to be the directory of the project output.
	std::string tempPath = "add any additional directories here." + libraryLocation;
	tempPath = libraryLocation;

	//Load libary into handle instance
	//loadA loads out DLL into memory for the program
	mLoadedLibrary = LoadLibraryEx(tempPath.c_str(), NULL, 0);

	//validate
	if (!mLoadedLibrary)
	{
		std::cout << "Could not load library" << std::endl;
		aaaaaaaaaaaaaaaa();
	}
	else
	{
		std::cout << "Loaded custom script DLL successfully." << std::endl;
	}
}

ScriptObject* ScriptLoader::GetFunctionPtrFromLibrary(std::string externalClassName)
{
	if (mLoadedLibrary)
	{
		//GetPRocAddress returns the ptr to the function we are trying to find within the DLL instance.
		std::string functionName = "Create_" + externalClassName;
		scriptPtr DLLFunction = (scriptPtr)GetProcAddress(mLoadedLibrary, functionName.c_str());

		if (!DLLFunction)
		{
			aaaaaaaaaaaaaaaa();
			std::cout << "Error getting function from DLL." << std::endl;
			return nullptr;
		}

		//resolve function address
		ScriptObject* function = DLLFunction();
		return function;
	}
}

bool ScriptLoader::LoadScriptObjectToMap(std::string externalClassName)
{
    //HARDCODED : DLL Location from built project
    std::string dllLocation = "C://Users//ryem_//source//repos//hTech//Scripting//Project//test//Debug//test.dll";
	LoadCustomScriptDLL(dllLocation);

	if (mLoadedLibrary)
	{
		ScriptObject* scriptObject = GetFunctionPtrFromLibrary(externalClassName);
		if (scriptObject != nullptr)
		{
			mLoadedScriptMap.insert(std::make_pair(externalClassName, scriptObject));
			return true;
		}
	}

    return false;
}

ScriptObject* ScriptLoader::GetScriptObject(std::string externalClassName)
{
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 4;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);


    if (mLoadedScriptMap.find(externalClassName) == mLoadedScriptMap.end())
    {
        if (LoadScriptObjectToMap(externalClassName) == false)
        {
			return nullptr;
        }
    }

    //todo : log not found.
	return mLoadedScriptMap.find(externalClassName)->second;
}

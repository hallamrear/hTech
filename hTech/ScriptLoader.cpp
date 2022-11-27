#include "pch.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"
#include "Log.h"
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
		Log::LogMessage(LogLevel::LOG_ERROR, "Could not load library");
	}
	else
	{
		Log::LogMessage(LogLevel::LOG_MESSAGE, "Loaded custom script DLL successfully.");
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
			Log::LogMessage(LogLevel::LOG_ERROR, "Error getting function from DLL.");
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

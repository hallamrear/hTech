#include "pch.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"
#include "Log.h"
#include <unordered_map>

static HINSTANCE m_LoadedLibraryInstance;
HINSTANCE ScriptLoader::m_LoadedLibraryInstance = nullptr;
std::unordered_map<std::string, ScriptObject*> ScriptLoader::m_LoadedScriptMap = std::unordered_map<std::string, ScriptObject*>();

typedef ScriptObject* (__stdcall* scriptPtr)();

ScriptLoader::~ScriptLoader()
{
	UnloadLibrary();
}

void ScriptLoader::UnloadLibrary()
{
	if (m_LoadedLibraryInstance)
	{
		FreeLibrary(m_LoadedLibraryInstance);
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
	m_LoadedLibraryInstance = LoadLibraryEx(tempPath.c_str(), NULL, 0);

	//validate
	if (!m_LoadedLibraryInstance)
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
	if (m_LoadedLibraryInstance)
	{
		//GetPRocAddress returns the ptr to the function we are trying to find within the DLL instance.
		std::string functionName = "Create_" + externalClassName;
		scriptPtr DLLFunction = (scriptPtr)GetProcAddress(m_LoadedLibraryInstance, functionName.c_str());

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

	if (m_LoadedLibraryInstance)
	{
		ScriptObject* scriptObject = GetFunctionPtrFromLibrary(externalClassName);
		if (scriptObject != nullptr)
		{
			m_LoadedScriptMap.insert(std::make_pair(externalClassName, scriptObject));
			return true;
		}
	}

    return false;
}

ScriptObject* ScriptLoader::GetScriptObject(std::string externalClassName)
{
    if (m_LoadedScriptMap.find(externalClassName) == m_LoadedScriptMap.end())
    {
        if (LoadScriptObjectToMap(externalClassName) == false)
        {
			std::string str = "Script Object not found: " + externalClassName;
			Log::LogMessage(LogLevel::LOG_ERROR, str);
			return nullptr;
        }
    }

	return m_LoadedScriptMap.find(externalClassName)->second;
}

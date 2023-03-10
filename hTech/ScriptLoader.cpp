#include "pch.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"
#include "Console.h"
#include <unordered_map>
#include <filesystem>
#include "ProjectLoader.h"
#include "Component_Script.h"

ScriptLoader* ScriptLoader::m_Instance = nullptr;

ScriptLoader::~ScriptLoader()
{
	UnloadLibrary();
}

bool ScriptLoader::IsLibraryLoaded()
{
	return Get()->IsLibraryLoaded_Impl();
}

void ScriptLoader::Reload(bool rebuildDLL)
{
	return Get()->Reload_Impl(rebuildDLL);
}

ScriptObject* ScriptLoader::GetScriptObject(Entity* entityFromComponent, std::string externalClassName)
{
	return Get()->GetScriptObject_Impl(entityFromComponent, externalClassName);
}

void ScriptLoader::AddScriptToReloadTracking(ScriptComponent* component)
{
	return Get()->AddObserver((Observer*)component);
}

void ScriptLoader::RemoveScriptFromReloadTracking(ScriptComponent* component)
{
	return Get()->RemoveObserver((Observer*)component);
}

bool ScriptLoader::IsLibraryLoaded_Impl()
{
	return m_IsLibraryLoaded;
}

void ScriptLoader::UnloadLibrary()
{
	return Get()->UnloadLibrary_Impl();
}

void ScriptLoader::UnloadLibrary_Impl()
{
	if (m_LoadedLibraryInstance)
	{
		m_ScriptCreationFunctionMap.clear();

		FreeLibrary(m_LoadedLibraryInstance);
		m_IsLibraryLoaded = false;
	}
}

ScriptLoader* ScriptLoader::Get()
{
	if (m_Instance == nullptr)
		m_Instance = new ScriptLoader();

	return m_Instance;
}

ScriptLoader::ScriptLoader() : ObserverSubject()
{

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
	m_LoadedLibraryInstance = LoadLibrary(tempPath.c_str());

	//validate
	if (!m_LoadedLibraryInstance)
	{
		Console::LogMessage(LogLevel::LOG_ERROR, "Could not load library");
		m_IsLibraryLoaded = false;
	}
	else
	{
		Console::LogMessage(LogLevel::LOG_MESSAGE, "Loaded custom script DLL successfully.");
		m_IsLibraryLoaded = true;
	}
}

scriptPtr ScriptLoader::GetScriptObjectCreationFunctionFromLibrary(std::string scriptName)
{
	if (m_LoadedLibraryInstance)
	{
		//GetPRocAddress returns the ptr to the function we are trying to find within the DLL instance.
		std::string functionName = "Create_" + scriptName;
		scriptPtr DLLFunction = (scriptPtr)GetProcAddress(m_LoadedLibraryInstance, functionName.c_str());

		if (!DLLFunction)
		{
			Console::LogMessage(LogLevel::LOG_ERROR, "Error getting function from DLL.");
			return nullptr;
		}

		//return function address
		return DLLFunction;
	}
}

bool ScriptLoader::LoadScriptObjectToMap(std::string functionName)
{
	if (!ProjectLoader::HasProjectLoaded())
		return false;

	std::string projectName = ProjectLoader::GetLoadedProjectName();

	std::string scriptsLocation;
	ProjectLoader::GetEngineProjectsLocation(scriptsLocation);
	scriptsLocation += projectName + "\\" + projectName + "\\" + projectName + ".dll";

	LoadCustomScriptDLL(scriptsLocation);

	if (m_LoadedLibraryInstance)
	{
		scriptPtr scriptBuildingFunction = GetScriptObjectCreationFunctionFromLibrary(functionName);
		if (scriptBuildingFunction != nullptr)
		{
			m_ScriptCreationFunctionMap.insert(std::make_pair(functionName, scriptBuildingFunction));
			return true;
		}
	}

    return false;
}

void ScriptLoader::Reload_Impl(bool rebuildDLL)
{
	UnloadLibrary();

	std::string projectLocation;
	ProjectLoader::GetEngineProjectsLocation(projectLocation);
	std::string projectName = ProjectLoader::GetLoadedProjectName();
	projectLocation += projectName + "\\" + projectName;
	std::string scriptsLocation = projectLocation;
	
	if (rebuildDLL)
	{
		projectLocation += ".sln";
		std::string buildCommand = "msbuild.exe " + projectLocation + " /t:Clean;Rebuild /property:Configuration=Debug -m && pause";
		system(buildCommand.c_str());
	}

	scriptsLocation += "\\" + projectName + ".dll";
	LoadCustomScriptDLL(scriptsLocation);

	NotifyAll();
}

ScriptObject* ScriptLoader::GetScriptObject_Impl(Entity* entityFromComponent, std::string scriptDLLName)
{
	if (m_ScriptCreationFunctionMap.find(scriptDLLName) == m_ScriptCreationFunctionMap.end())
	{
		if (LoadScriptObjectToMap(scriptDLLName) == false)
		{
			std::string str = "Cannot find function to create script object : " + scriptDLLName;
			Console::LogMessage(LogLevel::LOG_ERROR, str);
			return nullptr;
		}
	}

	ScriptObject* script = m_ScriptCreationFunctionMap.find(scriptDLLName)->second(entityFromComponent);

	if (script)
	{
		return script;
	}

	return nullptr;
}

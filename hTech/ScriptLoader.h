#pragma once

class ScriptObject;

class ScriptLoader
{
private:
	ScriptLoader();
	~ScriptLoader();

	static HINSTANCE mLoadedLibrary;
	static std::unordered_map<std::string, ScriptObject*> mLoadedScriptMap;

	static void UnloadLibrary();
	static void LoadCustomScriptDLL(std::string libraryLocation);
	static ScriptObject* GetFunctionPtrFromLibrary(std::string scriptClassName);

	static bool LoadScriptObjectToMap(std::string externalClassName);

public:
	static ScriptObject* GetScriptObject(std::string externalClassName);
};


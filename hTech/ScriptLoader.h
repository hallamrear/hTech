#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef _WINDEF_
class HINSTANCE__; // Forward or never
typedef HINSTANCE__* HINSTANCE;
#endif

class HTECH_FUNCTION_EXPORT ScriptObject;

class HTECH_FUNCTION_EXPORT ScriptLoader
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


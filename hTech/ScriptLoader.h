#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef _WINDEF_
class HINSTANCE__; // Forward or never
typedef HINSTANCE__* HINSTANCE;
#endif

class Entity;

class HTECH_FUNCTION_EXPORT ScriptObject;
typedef ScriptObject* (__cdecl* scriptPtr)(Entity*);

class HTECH_FUNCTION_EXPORT ScriptLoader
{
private:
	ScriptLoader();
	~ScriptLoader();

	static std::vector<ScriptObject*> m_LoadedScriptObjects;
	static HINSTANCE m_LoadedLibraryInstance;
	static std::unordered_map<std::string, scriptPtr> m_ScriptCreationFunctionMap;
	static bool m_IsLibraryLoaded;

	static void LoadCustomScriptDLL(std::string libraryLocation);
	static scriptPtr GetScriptObjectCreationFunctionFromLibrary(std::string scriptClassName);

	static bool LoadScriptObjectToMap(std::string externalClassName);

public:
	static bool IsLibraryLoaded();
	static void UnloadLibrary();
	static void Reload();
	static ScriptObject* GetScriptObject(Entity* entityFromComponent, std::string externalClassName);
};


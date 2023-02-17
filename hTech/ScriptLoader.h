#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)

#ifndef _WINDEF_
class HINSTANCE__; // Forward or never
typedef HINSTANCE__* HINSTANCE;
#endif

#include "ObserverSubject.h"

class Entity;
class ScriptComponent;

class HTECH_FUNCTION_EXPORT ScriptObject;
typedef ScriptObject* (__cdecl* scriptPtr)(Entity*);

class HTECH_FUNCTION_EXPORT ScriptLoader : public ObserverSubject
{
private:	
	static ScriptLoader* m_Instance;

	
	std::vector<ScriptObject*> m_LoadedScriptObjects;
	HINSTANCE m_LoadedLibraryInstance;
	std::unordered_map<std::string, scriptPtr> m_ScriptCreationFunctionMap;
	bool m_IsLibraryLoaded;
	void LoadCustomScriptDLL(std::string libraryLocation);
	scriptPtr GetScriptObjectCreationFunctionFromLibrary(std::string scriptClassName);
	bool LoadScriptObjectToMap(std::string externalClassName);
	bool IsLibraryLoaded_Impl();
	void Reload_Impl(bool rebuildDLL);
	ScriptObject* GetScriptObject_Impl(Entity* entityFromComponent, std::string externalClassName);
	void UnloadLibrary_Impl();

	//static ScriptLoader& Get();
	static ScriptLoader* Get();

protected:
	ScriptLoader();

public:
	~ScriptLoader();
	static void UnloadLibrary();
	static bool IsLibraryLoaded();
	static void Reload(bool rebuildDLL);
	static ScriptObject* GetScriptObject(Entity* entityFromComponent, std::string externalClassName);
	static void AddScriptToReloadTracking(ScriptComponent* observer);
	static void RemoveScriptFromReloadTracking(ScriptComponent* observer);
};


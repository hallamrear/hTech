#pragma once

#define StartScript(C) _StartScript(C)
#define _StartScript(C) class C : public Script
#define EndScript(C) _EndScript(C)
#define _EndScript(C) ;extern "C" { __declspec(dllexport) Script* CreateScript()	{ return new C(); } }
#define publicFunction _publicFunction
#define _publicFunction public: inline void
#define privateFunction _privateFunction
#define _privateFunction private: inline void

class ScriptObject
{
	

public:
	virtual void Update(float deltaTime) = 0;
};

class ScriptLoader
{
private:
	//static std::unordered_map<std::string, ScriptObject*> mLoadedScriptMap;
	static bool LoadScriptObjectToMap(std::string externalClassName);

public:
	static ScriptObject* GetScriptObject(std::string externalClassName);
};


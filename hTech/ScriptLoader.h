#pragma once

class ScriptObject
{

};

class ScriptLoader
{
private:
	std::unordered_map<std::string, ScriptObject*> mLoadedScriptMap = std::unordered_map<std::string, ScriptObject*>();
	ScriptObject* LoadScriptObject_Impl();

public:
	static ScriptObject* LoadScriptObject();
};


#include "pch.h"
#include "ScriptLoader.h"
#include <unordered_map>

bool ScriptLoader::LoadScriptObjectToMap(std::string externalClassName)
{
    //todo : remove hardcoding
    std::string dllLocation = "";



    return false;
}

ScriptObject* ScriptLoader::GetScriptObject(std::string externalClassName)
{
    //if (mLoadedScriptMap.find(externalClassName) != mLoadedScriptMap.end())
    //{
    //    if (LoadScriptObjectToMap(externalClassName))
    //    {
    //        return mLoadedScriptMap.find(externalClassName)->second;
    //    }
    //}

    //todo : log not found.
    return nullptr;
}

#pragma once
#include "Component.h"
#include "ScriptLoader.h"

class ScriptComponent :
    public Component
{
private:
    static ScriptObject* mScriptObject;

public:
    ScriptComponent(std::string externalClassName);
    ~ScriptComponent();

    void Update(float deltaTime);
};


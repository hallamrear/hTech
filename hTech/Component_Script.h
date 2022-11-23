#pragma once
#include "Component.h"
#include "ScriptLoader.h"

class ScriptComponent :
    public Component
{
private:
    ScriptObject* mScriptObject; 
    
protected:
    virtual void Destroy()
    {

    }

public:
    ScriptComponent(Entity& entity);
    ~ScriptComponent();

    void Update(float deltaTime);
};


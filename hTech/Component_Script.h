#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"
#include "ScriptLoader.h"

class HTECH_FUNCTION_EXPORT ScriptComponent :
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


#include "pch.h"
#include "Component_Script.h"
#include "Entity.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"

ScriptComponent::ScriptComponent(Entity& entity) : Component("Script Component", entity)
{
	std::string expectedName = entity.GetName();
	mScriptObject = ScriptLoader::GetScriptObject(expectedName);
}

ScriptComponent::~ScriptComponent()
{

}

void ScriptComponent::Update(float deltaTime)
{
	if (mScriptObject)
	{
		mScriptObject->Update(deltaTime);
	}
}
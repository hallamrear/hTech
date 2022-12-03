#include "pch.h"
#include "Component_Script.h"
#include "Collision.h"
#include "Entity.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"

void ScriptComponent::Destroy()
{
	if (mScriptObject)
	{
		//IMPLEMENT Proper destruction of script object.
		//mScriptObject->Destroy();
		//mScriptObject = nullptr;
	}
}

void ScriptComponent::RenderProperties()
{
	ImGui::Text("Script Components run out of .h files under the same name as the object, compiled via the engine.");
	ImGui::Text("There's really not much to write here that isn't adding a code editor and that feels abit much.");

	//IMPLEMENT In-Engine code editor.
}

void ScriptComponent::Update(float deltaTime)
{
	if (mScriptObject)
	{
		mScriptObject->Update(deltaTime);
	}
}

ScriptComponent::ScriptComponent(Entity& entity) : Component("Script Component", entity)
{
	std::string expectedName = entity.GetName();
	mScriptObject = ScriptLoader::GetScriptObject(expectedName);
}

ScriptComponent::~ScriptComponent()
{
	Destroy();
}

void ScriptComponent::Start()
{
	if (mScriptObject)
	{
		mScriptObject->Start();
	}
}

void ScriptComponent::OnEnable()
{
	if (mScriptObject)
	{
		mScriptObject->OnEnable();
	}
}

void ScriptComponent::OnDisable()
{
	if (mScriptObject)
	{
		mScriptObject->OnDisable();
	}
}

void ScriptComponent::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	if (mScriptObject)
	{
		mScriptObject->OnCollision(manifold, other);
	}
}

void ScriptComponent::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	if (mScriptObject)
	{
		mScriptObject->OnOverlap(manifold, other);
	}
}

ScriptObject const * ScriptComponent::GetScriptObject()
{
	if (mScriptObject)
		return mScriptObject;
	else
		return nullptr;
}

void ScriptComponent::Serialize(Serializer& writer) const
{
	//I don't think I need to initialise this because 
	//all of the script values will be in the script object
	//when it's created.
}

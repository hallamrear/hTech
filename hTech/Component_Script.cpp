#include "pch.h"
#include "Component_Script.h"
#include "Collision.h"
#include "Entity.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"

void ScriptComponent::Destroy()
{
	if (m_ScriptObject)
	{
		//IMPLEMENT Proper destruction of script object.
		//m_ScriptObject->Destroy();
		//m_ScriptObject = nullptr;
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
	if (m_ScriptObject)
	{
		m_ScriptObject->Update(deltaTime);
	}
}

ScriptComponent::ScriptComponent(Entity& entity) : Component("Script Component", entity)
{
	std::string expectedName = entity.GetName();
	m_ScriptObject = ScriptLoader::GetScriptObject(expectedName);
}

ScriptComponent::~ScriptComponent()
{
	Destroy();
}

void ScriptComponent::Start()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->Start();
	}
}

void ScriptComponent::OnEnable()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->OnEnable();
	}
}

void ScriptComponent::OnDisable()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->OnDisable();
	}
}

void ScriptComponent::OnCollision(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	if (m_ScriptObject)
	{
		m_ScriptObject->OnCollision(manifold, other);
	}
}

void ScriptComponent::OnOverlap(const CollisionManifold& manifold, RigidbodyComponent& other)
{
	if (m_ScriptObject)
	{
		m_ScriptObject->OnOverlap(manifold, other);
	}
}

ScriptObject const * ScriptComponent::GetScriptObject()
{
	if (m_ScriptObject)
		return m_ScriptObject;
	else
		return nullptr;
}

void ScriptComponent::Serialize(Serializer& writer) const
{
	Component::Serialize(writer);
	//I don't think I need to initialise this because 
	//all of the script values will be in the script object
	//when it's created.
}

void ScriptComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);
	//See serialize function
}
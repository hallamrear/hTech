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
		m_ScriptObject->Destroy();
		m_ScriptObject = nullptr;
	}
}

void ScriptComponent::RenderProperties()
{
	if (ImGui::Button("Reload DLL"))
	{
		ScriptLoader::Reload();
		m_ScriptObject = nullptr;
		m_ScriptObject = ScriptLoader::GetScriptObject(&m_ParentEntity, m_ScriptReferenceName);
	}
	ImGui::SameLine();
	if (ImGui::Button("Unload DLL"))
	{
		ScriptLoader::UnloadLibrary();
		m_ScriptObject = nullptr;
		m_ScriptObject = ScriptLoader::GetScriptObject(&m_ParentEntity, m_ScriptReferenceName);
	}
	ImGui::SameLine();

	std::string str = m_ScriptReferenceName;
	ImGui::InputText("Class Name: ", &str);
	if (str != m_ScriptReferenceName)
	{
		m_ScriptReferenceName = str;

		if (m_ScriptObject)
		{
			m_ScriptObject->Destroy();
			m_ScriptObject = nullptr;
		}
		else
		{
			m_ScriptObject = ScriptLoader::GetScriptObject(&m_ParentEntity, m_ScriptReferenceName);
		}
	}
}

void ScriptComponent::Update(float deltaTime)
{
	if (!ScriptLoader::IsLibraryLoaded())
	{
		if (m_ScriptObject)
		{
			m_ScriptObject->Destroy();
			delete m_ScriptObject;
			m_ScriptObject = nullptr;
		}
	}

	if (m_ScriptObject)
	{
		m_ScriptObject->Update(deltaTime);
	}
}

ScriptComponent::ScriptComponent(Entity& entity) : Component("Script Component", entity)
{
	//std::string expectedName = entity.GetName();
	//m_ScriptObject = ScriptLoader::GetScriptObject(expectedName);
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
#include "pch.h"
#include "Component_Script.h"
#include "Collision.h"
#include "Entity.h"
#include "ScriptLoader.h"
#include "ScriptObject.h"
#include "Game.h"

void ScriptComponent::Destroy()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->Destroy();
		m_ScriptObject = nullptr;
	}
}

void ScriptComponent::RenderProperties()
{
	ImGui::BeginDisabled(Game::GetGameState() == GAME_STATE::RUNNING);

	if (ImGui::Button("Rebuild DLL"))
	{
		ScriptLoader::Reload(true);
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload DLL"))
	{
		ScriptLoader::Reload(false);
	}
	ImGui::Separator();
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

	ImGui::EndDisabled();
}

void ScriptComponent::Update(float deltaTime)
{
	if (Game::GetGameState() != GAME_STATE::RUNNING)
		return;

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

ScriptComponent::ScriptComponent(Entity& entity) : Component("Script Component", entity), Observer()
{
	m_ScriptObject = nullptr;
	ScriptLoader::AddScriptToReloadTracking(this);
}

ScriptComponent::~ScriptComponent()
{
	ScriptLoader::RemoveScriptFromReloadTracking(this);
	Destroy();
}

void ScriptComponent::Start()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->Start();
	}
}

void ScriptComponent::Reset()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->Reset();
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
	writer.String("ClassName"); writer.String(m_ScriptReferenceName.c_str());
}

void ScriptComponent::Deserialize(SerializedValue& value)
{
	Component::Deserialize(value);
	if (value["ClassName"].IsString())
	{
		m_ScriptReferenceName = value["ClassName"].GetString();
		m_ScriptObject = ScriptLoader::GetScriptObject(&m_ParentEntity, m_ScriptReferenceName);
	}
}

void ScriptComponent::OnNotify()
{
	if (m_ScriptObject)
	{
		m_ScriptObject->Destroy();
		m_ScriptObject = nullptr;
	}

	m_ScriptObject = ScriptLoader::GetScriptObject(&m_ParentEntity, m_ScriptReferenceName);
}

void ScriptComponent::SetClass(const std::string& name)
{
	m_ScriptReferenceName = name;
	OnNotify();
}

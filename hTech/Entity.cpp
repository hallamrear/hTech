#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Component.h"
#include "Camera.h" //Included for WorldToScreen when rendering an entity with no components.

#include "Component_Transform.h"
#include "Component_Sprite.h"
#include "Component_Animation.h"
#include "Component_Script.h"
#include "Component_Rigidbody.h"
#include "rapidjson/rapidjson.h"
#include "World.h"

Entity::Entity(Transform SpawnTransform, std::string Name, Entity* Parent)
{
	m_IsEnabled = true;
	m_Components = std::vector<Component*>();
	AddComponent<TransformComponent>();
	GetTransform() = SpawnTransform;
	m_IsWaitingToBeDestroyed = false;
	m_Name = Name;
	m_IsAlive = true;

	if (Parent != nullptr)
	{
		SetParent(Parent);
	}
}

Entity::~Entity()
{
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		delete m_Components[i];
		m_Components[i] = nullptr;
	}

	m_Components.clear();
}

void Entity::Update(float DeltaTime)
{
	for (int i = 0; i != m_Components.size(); i++)
	{
		if (m_Components[i]->GetIsEnabled() == true)
		{
			m_Components[i]->Update(DeltaTime);
		}
	}
}

void Entity::Render()
{
	SDL_Renderer& renderer = *Game::Renderer;

	for (int i = 0; i != m_Components.size(); i++)
	{
		if (m_Components[i]->GetIsEnabled() == true)
		{
			m_Components[i]->Render(renderer);
		}
	}

	Vector2 pos = Camera::WorldToScreen(GetTransform().Position);
	SDL_Rect rect{ (int)pos.X - 2, (int)pos.Y - 2, 4, 4 };
	float a = 255;

	if (m_IsEnabled == false)
		a = 100;

	SDL_SetRenderDrawColor(&renderer, 0, 255, 0, a);
	SDL_RenderDrawRect(&renderer, &rect);
}

void Entity::RenderProperties()
{
	//This is called from editor window and so you can call imgui items directly.
	static bool enabled;
	enabled = m_IsEnabled;
	ImGui::Checkbox("Enabled", &enabled);
	SetEnabled(enabled);

	std::string str = m_Name;
	ImGui::InputText("Name: ", &str);	
	if (str != m_Name)
	{
		SetName(str);
	}
	ImGui::Text("Component Count: %i", m_Components.size());
	ImGui::Text("Alive: %i", m_IsAlive);
	ImGui::Text("Being Destroyed?: %i", m_IsWaitingToBeDestroyed);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_None))
	{
		ImGui::Separator(); ImGui::Spacing();

		if (ImGui::BeginPopupContextItem("AddComponentPopup"))
		{
			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Selectable("Transform Component"))
			{
				AddComponent<TransformComponent>();
			}

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Selectable("Sprite Component"))
			{
				AddComponent<SpriteComponent>();
			}

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Selectable("Animation Component"))
			{
				AddComponent<AnimationComponent>();
			}

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Selectable("Script Component"))
			{
				AddComponent<ScriptComponent>();
			}

			ImGui::SetNextItemWidth(-FLT_MIN);
			if (ImGui::Selectable("Rigidbody Component"))
			{
				AddComponent<RigidbodyComponent>();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupContextItem("RemoveComponentPopup"))
		{
			for (size_t i = 1; i < m_Components.size(); i++)
			{
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (ImGui::Selectable(m_Components[i]->GetComponentName().c_str()))
				{
					if (dynamic_cast<SpriteComponent*>(m_Components[i]))    { RemoveComponent<SpriteComponent>();	continue; }
					if (dynamic_cast<AnimationComponent*>(m_Components[i])) { RemoveComponent<AnimationComponent>();	continue; }
					if (dynamic_cast<ScriptComponent*>(m_Components[i]))    { RemoveComponent<ScriptComponent>();	continue; }
					if (dynamic_cast<RigidbodyComponent*>(m_Components[i])) { RemoveComponent<RigidbodyComponent>(); continue; }
					
				}
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}
		ImGui::SameLine();
		if (ImGui::Button("Remove Component"))
		{
			ImGui::OpenPopup("RemoveComponentPopup");
		}

		ImGui::Separator(); ImGui::Spacing();

		bool isComponentEnabled = false;
		std::string label = "";
		for (size_t i = 0; i < m_Components.size(); i++)
		{
			if (m_Components[i]->GetComponentName() != "Transform Component")
			{
				label = "##" + m_Components[i]->GetComponentName() + "componentEnabledCheckbox";
				bool isComponentEnabled = m_Components[i]->GetIsEnabled();
				ImGui::Checkbox(label.c_str(), &isComponentEnabled);
				m_Components[i]->SetIsEnabled(isComponentEnabled);
				ImGui::SameLine();
			}

			if (ImGui::CollapsingHeader(m_Components[i]->GetComponentName().c_str(), ImGuiTreeNodeFlags_None))
			{
				m_Components[i]->RenderProperties();
			}
		}
	}

}

void Entity::SetEnabled(const bool state)
{
	if (state == m_IsEnabled)
		return;

	ScriptComponent* script = GetComponent<ScriptComponent>();
	if (script)
	{
		//if now false and was true, we have been disabled.
		//what kind of disabled?
		//Leg disabled.
		if (state == false && m_IsEnabled == true)
		{
			script->OnDisable();
		}
		else
		{
			script->OnEnable();
		}
	}

	m_IsEnabled = state;
}

Entity* Entity::GetParent()
{
	//TODO : Implement Parenting
	return nullptr;
}

void Entity::SetParent(Entity* entity)
{
	//TODO : Implement Parenting
}

bool Entity::GetIsBeingDestroyed() const
{
	return m_IsWaitingToBeDestroyed;
}

void Entity::Destroy()
{
	m_IsWaitingToBeDestroyed = true;
}

Transform& Entity::GetTransform()
{
	return GetComponent<TransformComponent>()->GetTransform();
}

void Entity::ClampRotation()
{
	GetTransform().Rotation = fmod(GetTransform().Rotation, 360.0f);
}

const std::string& Entity::GetName() const
{
	return m_Name;
}

void Entity::SetName(const std::string& name)
{
	if (name != m_Name)
	{
		m_Name = name;
		World::UpdateHashmapNames();
	}
}

void Entity::Serialize(Serializer& writer) const
{
	writer.StartObject();

	writer.String("Name");	  writer.String(m_Name.c_str());
	writer.String("IsAlive"); writer.Bool(m_IsAlive);
	writer.String("IsEnabled"); writer.Bool(m_IsEnabled);

	writer.String("Components");
	writer.StartArray();
	for (size_t i = 0; i < m_Components.size(); i++)
	{
		writer.StartObject();
		m_Components[i]->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();
}

void Entity::Deserialize(SerializedValue& serializedEntity)
{
	if (serializedEntity["IsAlive"].IsBool())
	{
		m_IsAlive = serializedEntity["IsAlive"].GetBool();
	}

	if (serializedEntity["IsEnabled"].IsBool())
	{
		m_IsEnabled = serializedEntity["IsEnabled"].GetBool();
	}

	if (serializedEntity.HasMember("Components"))
	{
		SerializedValue components = serializedEntity["Components"].GetArray();

		for (size_t i = 0; i < components.Size(); i++)
		{
			if (components[i]["Component Name"].IsString())
			{
				std::string name = components[i]["Component Name"].GetString();

				Component* component = nullptr;

				if (name == "Transform Component")
				{
					component = GetComponent<TransformComponent>();
				}
				else if (name == "Sprite Component")
				{
					AddComponent<SpriteComponent>();
					component = GetComponent<SpriteComponent>();
				}
				else if (name == "Rigidbody Component")
				{
					AddComponent<RigidbodyComponent>();
					component = GetComponent<RigidbodyComponent>();
				}
				else if (name == "Animation Component")
				{
					AddComponent<AnimationComponent>();
					component = GetComponent<AnimationComponent>();
				}
				else if (name == "Script Component")
				{
					AddComponent<ScriptComponent>();
					component = GetComponent<ScriptComponent>();
				}

				component->Deserialize(components[i]);
			}
		}
	}
}
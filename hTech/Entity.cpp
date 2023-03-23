#include "pch.h"
#include "Entity.h"
#include "Engine.h"
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

#include "IRenderer.h"

Entity::Entity(Transform SpawnTransform, std::string Name, Entity* Parent)
{
	m_IsEnabled = true;
	m_Layer = RENDER_LAYER::DEFAULT;
	m_Components = std::vector<Component*>();
	m_Children = std::vector<Entity*>();
	AddComponent<TransformComponent>();
	GetTransform() = SpawnTransform;
	m_IsWaitingToBeDestroyed = false;
	m_Name = Name;
	m_IsAlive = true;

	if (Parent != nullptr)
	{
		SetParent(Parent);
	}

	SetEntityRenderLayer(m_Layer);
}

Entity::~Entity()
{
	Engine::GetRenderer().GetRenderLayer(m_Layer).RemoveEntity(*this);

	for (size_t i = 0; i < m_Components.size(); i++)
	{
		delete m_Components[i];
		m_Components[i] = nullptr;
	}

	m_Components.clear();
}

void Entity::Update(float DeltaTime)
{
	if (m_IsEnabled && !m_IsWaitingToBeDestroyed)
	{
		for (int i = 0; i != m_Components.size(); i++)
		{
			if (m_Components[i]->GetIsEnabled() == true)
			{
				m_Components[i]->Update(DeltaTime);
			}
		}
	}
}

void Entity::Render(IRenderer& renderer)
{
	if (m_IsEnabled && !m_IsWaitingToBeDestroyed)
	{
		for (int i = 0; i != m_Components.size(); i++)
		{
			if (m_Components[i]->GetIsEnabled() == true)
			{
				m_Components[i]->Render(renderer);
			}
		}
	}

	if (Engine::GetGameState() != GAME_STATE::RUNNING)
	{
		Vector2 position = Camera::WorldToScreen(GetTransform().Position);
		ScreenRectangle rect = ScreenRectangle(position.X, position.Y, 8, 8);
		float a = 255;

		if (m_IsEnabled == false)
			a = 100;

		renderer.SetPrimativeDrawColour(Colour(0, 255, 0, a));
		renderer.Render_ScreenSpaceRectangle(rect, false);
	}
}

void Entity::RenderProperties()
{
	//This is called from editor window and so you can call imgui items directly.
	static bool enabled;
	enabled = m_IsEnabled;
	ImGui::Checkbox("Enabled", &enabled);
	SetEnabled(enabled);
	
	std::string parentName = "";
	if (m_Parent) parentName = m_Parent->GetName();
	ImGui::InputText("Parent: ", &parentName);
	Entity* parent = World::GetEntityByName(parentName);

	if (parent != nullptr)
	{
		if (parent != GetParent())
		{
			SetParent(parent);
		}
	}

	std::string str = m_Name;
	ImGui::InputText("Name: ", &str);	
	if (str != m_Name)
	{
		SetName(str);
	}

	static const std::string layerStrs[5] = { "Background", "Default", "Foreground", "UI", "Debug Layer" };
	std::string currentItem = layerStrs[(int)m_Layer];

	ImGui::Separator();
	if (ImGui::BeginCombo("Render Sort Layer", currentItem.c_str()))
	{
		for (int n = 0; n < IM_ARRAYSIZE(layerStrs); n++)
		{
			bool is_selected = (currentItem == layerStrs[n].c_str()); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(layerStrs[n].c_str(), is_selected))
			{
				if (currentItem != layerStrs[n])
				{
					Engine::GetRenderer().GetRenderLayer(m_Layer).RemoveEntity(*this);
					currentItem = layerStrs[n];
					m_Layer = (RENDER_LAYER)n;
					Engine::GetRenderer().GetRenderLayer(m_Layer).AddEntity(*this);

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				}
			}
		}
		ImGui::EndCombo();
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
					if (dynamic_cast<RigidbodyComponent*>(m_Components[i])) { 
						RemoveComponent<RigidbodyComponent>(); 
						continue; }
					
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

void Entity::SetEnabled(const bool state, const bool alterChildren)
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

	if (alterChildren)
	{
		for (size_t i = 0; i < m_Children.size(); i++)
		{
			m_Children[i]->SetEnabled(state, alterChildren);
		}
	}
}

Entity* Entity::GetParent()
{
	return m_Parent;
}

void Entity::SetParent(Entity* entity)
{
	if (m_Parent != nullptr)
	{
		m_Parent->RemoveChild(this);
	}

	m_Parent = entity;
	
	if(m_Parent)
		m_Parent->AddChild(this);
}

void Entity::AddChild(Entity* entity)
{
	if (!entity)
		return;

	if (entity->GetParent() != nullptr)
	{
		entity->GetParent()->RemoveChild(entity);
	}

	m_Children.push_back(entity);
}

void Entity::RemoveChild(Entity* entity)
{
	if (entity != nullptr && m_Children.size() > 0)
	{
		auto itr = std::find(m_Children.begin(), m_Children.end(), entity);

		if (itr != m_Children.end())
		{
			m_Children.erase(itr);	
			entity->SetParent(nullptr);
		}
	}
}

std::vector<Entity*>& Entity::GetChildren()
{
	return m_Children;
}

const RENDER_LAYER& Entity::GetEntityRenderLayer() const
{
	return m_Layer;
}

void Entity::SetEntityRenderLayer(const RENDER_LAYER& layer)
{
	Engine::GetRenderer().GetRenderLayer(m_Layer).RemoveEntity(*this);
	m_Layer = layer;
	Engine::GetRenderer().GetRenderLayer(m_Layer).AddEntity(*this);
}

bool Entity::GetIsBeingDestroyed() const
{
	return m_IsWaitingToBeDestroyed;
}

void Entity::Destroy()
{
	m_IsWaitingToBeDestroyed = true;
	SetEnabled(false);
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
	writer.String("Layer"); writer.Int((int)m_Layer);

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

	if (serializedEntity.HasMember("Layer"))
	{
		if (serializedEntity["Layer"].IsInt())
		{
			int layer = serializedEntity["Layer"].GetInt();
			SetEntityRenderLayer((RENDER_LAYER)layer);			
		}
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
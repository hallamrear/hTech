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

Entity::Entity(Transform SpawnTransform, std::string Name, Entity* Parent)
{
	IsEnabled = true;
	mComponents = std::vector<Component*>();
	AddComponent<TransformComponent>();
	GetTransform() = SpawnTransform;
	mIsWaitingToBeDestroyed = false;
	mName = Name;
	mIsAlive = true;

	if (Parent != nullptr)
	{
		SetParent(Parent);
	}
}

Entity::~Entity()
{
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		delete mComponents[i];
		mComponents[i] = nullptr;
	}

	mComponents.clear();
}

void Entity::Update(float DeltaTime)
{
	for (int i = 0; i != mComponents.size(); i++)
	{
		if (mComponents[i]->GetIsEnabled() == true)
		{
			mComponents[i]->Update(DeltaTime);
		}
	}
}

void Entity::Render()
{
	SDL_Renderer& renderer = *Game::Renderer;

	for (int i = 0; i != mComponents.size(); i++)
	{
		if (mComponents[i]->GetIsEnabled() == true)
		{
			mComponents[i]->Render(renderer);
		}
	}

	Vector2 pos = Camera::WorldToScreen(GetTransform().Position);
	SDL_Rect rect{ (int)pos.X - 2, (int)pos.Y - 2, 4, 4 };
	float a = 255;

	if (IsEnabled == false)
		a = 100;

	SDL_SetRenderDrawColor(&renderer, 0, 255, 0, a);
	SDL_RenderDrawRect(&renderer, &rect);
}

void Entity::RenderProperties()
{
	//This is called from editor window and so you can call imgui items directly.
	ImGui::Checkbox("Enabled", &IsEnabled);
	ImGui::InputText("Name: ", &mName);	
	ImGui::Text("Component Count: %i", mComponents.size());
	ImGui::Text("Alive: %i", mIsAlive);
	ImGui::Text("Being Destroyed?: %i", mIsWaitingToBeDestroyed);
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
			for (size_t i = 1; i < mComponents.size(); i++)
			{
				ImGui::SetNextItemWidth(-FLT_MIN);
				if (ImGui::Selectable(mComponents[i]->GetComponentName().c_str()))
				{
					if (dynamic_cast<SpriteComponent*>(mComponents[i]))    { RemoveComponent<SpriteComponent>();	continue; }
					if (dynamic_cast<AnimationComponent*>(mComponents[i])) { RemoveComponent<AnimationComponent>();	continue; }
					if (dynamic_cast<ScriptComponent*>(mComponents[i]))    { RemoveComponent<ScriptComponent>();	continue; }
					if (dynamic_cast<RigidbodyComponent*>(mComponents[i])) { RemoveComponent<RigidbodyComponent>(); continue; }
					
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
		for (size_t i = 0; i < mComponents.size(); i++)
		{
			if (mComponents[i]->GetComponentName() != "Transform Component")
			{
				label = "##" + mComponents[i]->GetComponentName() + "componentEnabledCheckbox";
				bool isComponentEnabled = mComponents[i]->GetIsEnabled();
				ImGui::Checkbox(label.c_str(), &isComponentEnabled);
				mComponents[i]->SetIsEnabled(isComponentEnabled);
				ImGui::SameLine();
			}

			if (ImGui::CollapsingHeader(mComponents[i]->GetComponentName().c_str(), ImGuiTreeNodeFlags_None))
			{
				mComponents[i]->RenderProperties();
			}
		}
	}

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
	return mIsWaitingToBeDestroyed;
}

void Entity::Destroy()
{
	mIsWaitingToBeDestroyed = true;
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
	return mName;
}

void Entity::Serialize(Serializer& writer) const
{
	writer.StartObject();

	writer.String("Name");	  writer.String(mName.c_str());
	writer.String("IsAlive"); writer.Bool(mIsAlive);
	writer.String("IsEnabled"); writer.Bool(IsEnabled);

	writer.String("Components");
	writer.StartArray();
	for (size_t i = 0; i < mComponents.size(); i++)
	{
		writer.StartObject();
		mComponents[i]->Serialize(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();
}

void Entity::Deserialize(SerializedValue& serializedEntity)
{
	if (serializedEntity["IsAlive"].IsBool())
	{
		mIsAlive = serializedEntity["IsAlive"].GetBool();
	}

	if (serializedEntity["IsEnabled"].IsBool())
	{
		IsEnabled = serializedEntity["IsEnabled"].GetBool();
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
#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Component_Transform.h"
#include "Camera.h" //Included for WorldToScreen when rendering an entity with no components.

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

	if (mComponents.size() <= 1)
	{
		Vector2 pos = Camera::WorldToScreen(GetTransform().Position);
		SDL_Rect rect{ (int)pos.X, (int)pos.Y, 4, 4 };
		SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
		SDL_RenderDrawRect(&renderer, &rect);
	}
}

Entity* Entity::GetParent()
{
	//TODO : Implement
	return nullptr;
}

void Entity::SetParent(Entity* entity)
{
	//TODO : Implement
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

#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Component_Transform.h"

Entity::Entity()
{
	IsEnabled = true;
	mComponents = std::vector<Component*>();
	AddComponent<TransformComponent>();
	mIsWaitingToBeDestroyed = false;
	Name = "unnamed";
	mIsAlive = true;
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

void Entity::Update(double DeltaTime)
{
	for (int i = 0; i < mComponents.size(); i++)
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

	for (int i = 0; i < mComponents.size(); i++)
	{
		if (mComponents[i]->GetIsEnabled() == true)
		{
			mComponents[i]->Render(renderer);
		}
	}
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
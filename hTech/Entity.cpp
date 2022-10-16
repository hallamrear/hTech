#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"
#include "Component_Transform.h"

Entity::Entity(std::string texture_path)
	: mRenderer(*Game::Renderer)
{
	IsEnabled = true;
	mComponents = std::vector<Component*>();
	AddComponent<TransformComponent>();
	mIsWaitingToBeDestroyed = false;
	Name = "unnamed";
	mIsAlive = true;

	if (texture_path != "")
		AssignTexture(texture_path);
	else
		mTexture = nullptr;
}

Entity::~Entity()
{
	//IMPLEMENT
	mTexture = nullptr;
}

void Entity::AssignTexture(const std::string& texture_path)
{
	mTexture = TextureCache::GetTexture(texture_path);
}

const Texture& Entity::GetTexture() const
{
	return *mTexture;
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

const SDL_Renderer& Entity::GetRendererReference()
{
	return mRenderer;
}

void Entity::ClampRotation()
{
	GetTransform().Rotation = fmod(GetTransform().Rotation, 360.0f);
}
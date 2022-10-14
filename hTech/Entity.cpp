#include "pch.h"
#include "Entity.h"
#include "Game.h"
#include "Texture.h"
#include "TextureCache.h"

Entity::Entity(std::string texture_path, Transform transform)
	: mRenderer(*Game::Renderer)
{
	mIsWaitingToBeDestroyed = false;

	Name = "unnamed";

	mTransform = transform;
	mIsAlive = true;

	if (texture_path != "")
		AssignTexture(texture_path);
	else
		mTexture = nullptr;
}

Entity::~Entity()
{
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

const SDL_Renderer& Entity::GetRendererReference()
{
	return mRenderer;
}

void Entity::ClampRotation()
{
	mTransform.Rotation = fmod(mTransform.Rotation, 360.0f);
}
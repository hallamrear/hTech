#include "pch.h"
#include "Entity.h"
#include "Component_Sprite.h"
#include "TextureCache.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Entity& entity) : Component("Sprite Component", entity)
{
	mTexture = nullptr;
	mIsFlipped = false;
}

SpriteComponent::~SpriteComponent()
{
	mTexture = nullptr;
}

void SpriteComponent::SetIsFlipped(bool state)
{
	mIsFlipped = state;
}

bool SpriteComponent::IsFlipped()
{
	return mIsFlipped;
}

void SpriteComponent::LoadTexture(std::string texture_path)
{
	mTexture = TextureCache::GetTexture(texture_path);
}

void SpriteComponent::UnloadTexture()
{
	if (mTexture)
	{
		mTexture = nullptr;
	}
}

void SpriteComponent::Update(float DeltaTime)
{

}

void SpriteComponent::Render(SDL_Renderer& renderer)
{
	if (mIsEnabled)
	{
		if (mTexture)
		{
			mTexture->Render(renderer, Parent.GetTransform().Position, Parent.GetTransform().Rotation, mIsFlipped);
		}
	}
}

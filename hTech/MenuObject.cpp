#include "pch.h"
#include "MenuObject.h"
#include "BoundingBox.h"
#include "Texture.h"

MenuObject::MenuObject(std::string texture_path)
	: Rigidbody(texture_path, PhysicsProperties())
{
	mIsStatic = true;
	mCollider = new BoundingBox(GetTransform().Position, (float)mTexture->Width, (float)mTexture->Height);
}

MenuObject::~MenuObject()
{

}

void MenuObject::Update(double deltaTime)
{
	if (mCollider)
		mCollider->Update(deltaTime);
}

void MenuObject::Render()
{
	if(GetIsAlive())
	{
		SDL_Rect destRect{};
		destRect.w = mTexture->Width;
		destRect.h = mTexture->Height;
		destRect.x = (int)(GetTransform().Position.X - (destRect.w / 2.0f));
		destRect.y = (int)(GetTransform().Position.Y - (destRect.h / 2.0f));
		SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, GetTransform().Rotation, NULL, SDL_FLIP_NONE);

		if (mCollider)
			mCollider->Render(mRenderer);
	}
}

#include "pch.h"
#include "MenuObject.h"
#include "BoundingBox.h"
#include "Texture.h"

MenuObject::MenuObject(std::string texture_path, Transform transform)
	: Rigidbody(texture_path, transform, PhysicsProperties())
{
	mIsStatic = true;
	mCollider = new BoundingBox(mTransform.Position, (float)mTexture->Width, (float)mTexture->Height);
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
		destRect.x = (int)(mTransform.Position.X - (destRect.w / 2.0f));
		destRect.y = (int)(mTransform.Position.Y - (destRect.h / 2.0f));
		SDL_RenderCopyEx(&mRenderer, &mTexture->GetSDLTexture(), NULL, &destRect, mTransform.Rotation, NULL, SDL_FLIP_NONE);

		if (mCollider)
			mCollider->Render(mRenderer);
	}
}

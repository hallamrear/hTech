#include "pch.h"
#include "Component_Animation.h"
#include "TextureCache.h"
#include "Texture.h"
#include "Entity.h"
#include "Game.h"

AnimationComponent::AnimationComponent(Entity& entity) : Component("Animation Controller", entity)
{
	mAnimationSheet = nullptr;
	mTimeElapsed = 0.0f;
	IsLooping = false;
	mDuration = 0.0f;
	mTotalFrames = 0;
	mTimeBetweenFrames = 0.0f;
	mAnimationCount = 0;
	mFrameSize.X = 0.0f;
	mFrameSize.Y = 0.0f;
	mHasFinished = false;
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::Update(float DeltaTime)
{
	if (mAnimationSheet)
	{
		if (mHasFinished == false)
		{
			mTimeElapsed += DeltaTime;

			if (mTimeElapsed > mDuration)
			{
				if (IsLooping)
				{
					mTimeElapsed = 0.0f;
					mCurrentFrame = 0;
				}
				else
				{
					mTimeElapsed = 0.0f;
					mCurrentFrame = 0;
					mHasFinished = true;
				}
			}
			else
			{
				mCurrentFrame = (unsigned int)(trunc(mTimeElapsed / mTimeBetweenFrames));
			}
		}
	}
}

void AnimationComponent::Render(SDL_Renderer& renderer)
{
	if (mAnimationSheet)
	{
		Transform& transform = Parent.GetTransform();
		Vector2 srcPos = Vector2((mFrameSize.X * mCurrentFrame) + (mFrameSize.X / 2.0f), (mFrameSize.Y * mCurrentAnimation) + mFrameSize.Y / 2.0f);
		mAnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, mFrameSize, IsFlipped);
	}
}

void AnimationComponent::LoadAnimationSheet(std::string sheet_path)
{
	if (mAnimationSheet)
	{
		mAnimationSheet = nullptr;
	}

	mAnimationSheet = TextureCache::GetTexture(sheet_path);
	
	RecalculateFrameAndAnimationData();

	mTimeElapsed = 0.0f;
	IsLooping = false;
	mDuration = 1.0f;
	mAnimationCount = 1;
	mTotalFrames = 4;
	mHasFinished = false;
}

void AnimationComponent::RecalculateFrameAndAnimationData()
{
	mTimeBetweenFrames = mDuration / (float)(mTotalFrames);
	mFrameSize = Vector2(mAnimationSheet->Width / (float)mTotalFrames, mAnimationSheet->Height / (float)mAnimationCount);
}

void AnimationComponent::SetAnimationCount(unsigned int animationCount)
{
	mAnimationCount = animationCount;
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetAnimationFrameCount(unsigned int frameCountPerAnimation)
{
	mTotalFrames = frameCountPerAnimation;
	RecalculateFrameAndAnimationData();
}

void AnimationComponent::SetDuration(float value)
{
	mDuration = value;
	RecalculateFrameAndAnimationData();
}

float AnimationComponent::GetTimeElapsed() const
{
	return mTimeElapsed;
}

bool AnimationComponent::HasFinished() const
{
	return mHasFinished;
}
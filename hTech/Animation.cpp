#include "pch.h"
#include "Animation.h"
#include "Game.h"
#include "TextureCache.h"

AnimationController::AnimationController(std::string sheetPath, unsigned int animationCount, unsigned int frameCountPerAnimation, float duration, bool looping)
{
	mAnimationSheet = TextureCache::GetTexture(sheetPath);
	mTimeElapsed = 0.0f;
	mIsLooping = looping;
	mDuration = duration;
	mTotalFrames = frameCountPerAnimation;
	mTimeBetweenFrames = mDuration / (float)(mTotalFrames);
	FrameSize = Vector2(mAnimationSheet->Width / (float)mTotalFrames, mAnimationSheet->Height / (float)animationCount);
	mHasFinished = false;
}

AnimationController::~AnimationController()
{
	mAnimationSheet = nullptr;
}

bool AnimationController::HasFinished()
{
	return mHasFinished;
}

void AnimationController::Start()
{
	mHasFinished = false;
	mCurrentFrame = 0;
	mTimeElapsed = 0.0f;
}

void AnimationController::SetAnimation(unsigned int animation)
{
	mCurrentAnimation = animation;
}

void AnimationController::Update(float DeltaTime)
{
	if (mAnimationSheet)
	{
		if (mHasFinished == false)
		{
			mTimeElapsed += DeltaTime;

			if (mTimeElapsed > mDuration)
			{
				if (mIsLooping)
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


void AnimationController::Render(SDL_Renderer& renderer, Transform transform)
{
	if (mAnimationSheet)
	{
		Vector2 srcPos = Vector2((FrameSize.X * mCurrentFrame) + (FrameSize.X / 2.0f), (FrameSize.Y * mCurrentAnimation) + FrameSize.Y / 2.0f);
		mAnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, FrameSize);
	}
}

void AnimationController::Render(SDL_Renderer& renderer, Transform transform, bool flipped)
{
	if (mAnimationSheet)
	{
		Vector2 srcPos = Vector2((FrameSize.X * mCurrentFrame) + (FrameSize.X / 2.0f), (FrameSize.Y * mCurrentAnimation) + FrameSize.Y / 2.0f);
		mAnimationSheet->Render(*Game::Renderer, transform.Position, transform.Rotation, srcPos, FrameSize, flipped);
	}
}

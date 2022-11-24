#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Texture.h"
#include "Transform.h"

class HTECH_FUNCTION_EXPORT AnimationController
{
	float mTimeBetweenFrames;
	float mTimeElapsed;
	float mDuration;
	unsigned int mCurrentFrame;
	Texture* mAnimationSheet;
	bool mIsLooping;
	unsigned int mCurrentAnimation;
	unsigned int mTotalFrames;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool mHasFinished;

public:
	Vector2 FrameSize;

	AnimationController(std::string sheetPath, unsigned int numberOfAnimations, unsigned int frameCount, float duration, bool looping);
	~AnimationController();

	bool HasFinished();
	void Start();
	void SetAnimation(unsigned int animation);
	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer, Transform transform);
	void Render(SDL_Renderer& renderer, Transform transform, bool flipped);
};
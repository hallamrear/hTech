#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"

struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Texture;

class HTECH_FUNCTION_EXPORT AnimationComponent
	: public Component
{
private:
	float mTimeBetweenFrames;
	float mTimeElapsed;
	float mDuration;
	unsigned int mCurrentFrame;
	Texture* mAnimationSheet;
	unsigned int mCurrentAnimation;
	unsigned int mTotalFrames;
	Vector2 mFrameSize;
	unsigned int mAnimationCount;
	unsigned int mAnimationFrameCount;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool mHasFinished;
	void RecalculateFrameAndAnimationData();

protected:

public:
	bool IsLooping;
	bool IsFlipped;

	void SetDuration(float value);
	float GetTimeElapsed() const;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool HasFinished() const;
	void LoadAnimationSheet(std::string sheet_path);
	void SetAnimationCount(unsigned int animationCount);
	void SetAnimationFrameCount(unsigned int frameCountPerAnimation);

	AnimationComponent(Entity& entity);
	~AnimationComponent();

	void Update(float DeltaTime);
	void Render(SDL_Renderer& renderer);
};


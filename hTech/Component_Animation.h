#pragma once
 #define HTECH_FUNCTION_EXPORT __declspec(dllexport)



#include "Component.h"

struct SDL_Renderer;
class HTECH_FUNCTION_EXPORT Texture;

class HTECH_FUNCTION_EXPORT AnimationComponent
	: public Component
{
private:
	std::string mUINewAnimationSheetString;
	float mTimeBetweenFrames;
	float mTimeElapsed;
	float mDuration;
	int mCurrentFrame;
	Texture* mAnimationSheet;
	int mCurrentAnimation;
	int mTotalFrames;
	Vector2 mFrameSize;
	int mAnimationCount;

	//If the animation does not loop, this will set to true when it has finished playing.
	bool mHasFinished;
	void RecalculateFrameAndAnimationData();

protected:
	void RenderProperties();

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

	void Serialize(Serializer& writer) const override;
};

